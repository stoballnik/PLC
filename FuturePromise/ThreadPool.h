#pragma once

#include <thread>
#include <vector>
#include <functional>

#include "Future.h"
#include "PoolQueue.h"


class ThreadPool {
public:
    explicit ThreadPool(int numThreads = std::thread::hardware_concurrency(), PoolQueue *customQueue = nullptr)
            : numThreads(numThreads),
                queue(customQueue ? customQueue : new PoolQueue(numThreads)),
                stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            queue->ReleaseThread(i);
            workers.emplace_back(threadFunction, std::ref(*queue), std::cref(stop), i);
        }
    }

    template<class T>
    Future<T> TryEnqueue(std::function<T()> task);

    template<class T, class... Args>
    Future<T> TryEnqueue(std::function<T(Args...)> task, Args... args);

    ~ThreadPool();

private:
    static void threadFunction(PoolQueue& queue, const std::atomic<bool> &stop, int idx);

    std::vector<std::thread> workers;
    std::unique_ptr<PoolQueue> queue;
    std::atomic<bool> stop;
    int numThreads;
};

inline void ThreadPool::threadFunction(PoolQueue &queue, const std::atomic<bool> &stop, int idx) {
    while (!stop) {
        queue.WaitTask(idx, [&]() {return bool(stop);});

        if (stop) {
            return;
        }

        queue.Task(idx)();
        queue.ReleaseThread(idx);
    }
}

template<class T>
Future<T> ThreadPool::TryEnqueue(std::function<T()> task) {
    std::shared_ptr<Promise<T>> result(new Promise<T>());

    std::function<void()> workerTask = [result, task] () {
        try {
            result->SetValue(new T(task()));
        } catch (std::exception_ptr e) {
            result->SetException(e);
        }
    };

    if (queue->TryEnqueue(workerTask)) {
        return result->GetFuture();
    }

    return Future<T>();
}

inline ThreadPool::~ThreadPool() {
    stop = true;
    for (int i = 0; i < numThreads; ++i) {
        queue->NotifyThread(i);
        workers[i].join();
    }
}

template<class T, class... Args>
Future<T> ThreadPool::TryEnqueue(std::function<T(Args...)> task, Args... args) {
    return TryEnqueue<T>(std::bind(task, args...));
}