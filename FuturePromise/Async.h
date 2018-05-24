#pragma once

#include "ThreadPool.h"


enum class Method {
    Async,
	Sync
};

template<class T, class... Args>
Future<T> Async(Method policy,
                        ThreadPool *pool,
                        std::function<T(Args...)> task,
                        Args ...args) {
    switch (policy) {
        case Method::Async:
        {
            assert(pool);
            auto future = pool->TryEnqueue<T, Args...>(task, args...);
            if (future)
                return future;
        }
        case Method::Sync:
        {
            std::shared_ptr<Promise<T>> result(new Promise<T>());

            auto simpleTask = std::bind(task, args...);

            auto init = [result, simpleTask]() {
                try {
                    result->SetValue(new T(simpleTask()));
                } catch (std::exception_ptr e) {
                    result->SetException(e);
                }
            };

            auto future = result->GetFuture();
            future.SetInitializer(init);

            return future;
        }
    }
}
