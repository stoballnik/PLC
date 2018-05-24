#pragma once

#include <mutex>
#include <vector>
#include <condition_variable>


class PoolQueue{
public:
    explicit PoolQueue(int numThreads) :
            numThreads(numThreads),
            isFree(numThreads),
            tasks(numThreads),
            taskGuards(numThreads),
            cvs(numThreads) {
    }

    bool TryEnqueue(std::function<void()> task);

    std::function<void()>& Task(int threadId);
    bool HasTask(int threadId);
    void ReleaseThread(int threadId);
    void WaitTask(int threadId, std::function<bool()> predicate);
    void NotifyThread(int threadId);

private:
    int numThreads;
    std::mutex mtx;
    std::vector<bool> isFree;
    std::vector<std::function<void()>> tasks;
    std::vector<std::mutex> taskGuards;
    std::vector<std::condition_variable> cvs;
};

bool PoolQueue::TryEnqueue(std::function<void()> task) {
	std::lock_guard<std::mutex> guard(mtx);
	for (int i = 0; i < numThreads; ++i) {
		if (isFree[i]) {
			tasks[i] = task;
			isFree[i] = false;
			NotifyThread(i);
			return true;
		}
	}
	return false;
}

std::function<void()>& PoolQueue::Task(int threadId) {
	assert(threadId < numThreads);
	return tasks[threadId];
}

bool PoolQueue::HasTask(int threadId) {
	assert(threadId < numThreads);
	return !isFree[threadId];
}

void PoolQueue::ReleaseThread(int threadId) {
	assert(threadId < numThreads);
	isFree[threadId] = true;
}

void PoolQueue::WaitTask(int threadId, std::function<bool()> predicate) {
	assert(threadId < numThreads);
	std::unique_lock<std::mutex> lock(taskGuards[threadId]);
	cvs[threadId].wait(lock, [&] {
		return HasTask(threadId) || predicate();
	});
}

void PoolQueue::NotifyThread(int threadId) {
	assert(threadId < numThreads);
	cvs[threadId].notify_one();
}
