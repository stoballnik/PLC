#pragma once

#include <memory>
#include <exception>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<class T>
class Promise;

template<class T>
class Future;

template<class T>
class SharedState {
public:
    SharedState() : exception(nullptr), data(nullptr), empty(true) {}

    void SetValue(T* newValue);
    void SetException(std::exception_ptr newException);
    bool Empty() const;
    std::shared_ptr<T> GetValue() const;
	std::exception_ptr GetException() const;

    friend class Promise<T>;
    friend class Future<T>;
private:
    std::shared_ptr<T> data;
    std::atomic<bool> empty;
	std::exception_ptr exception;
    std::mutex mtx;
    std::condition_variable cv;
};

template<class T>
void SharedState<T>::SetValue(T* newValue) {
    std::lock_guard<std::mutex> guard(mtx);
    data.reset(newValue);
    empty = false;
    cv.notify_all();
}

template<class T>
void SharedState<T>::SetException(std::exception_ptr newException) {
	std::lock_guard<std::mutex> guard(mtx);
	exception = newException;
	empty = false;
	cv.notify_all();
}

template<class T>
bool SharedState<T>::Empty() const {
    return bool(empty);
}

template<class T>
std::shared_ptr<T> SharedState<T>::GetValue() const {
    return data;
}

template<class T>
std::exception_ptr SharedState<T>::GetException() const {
	return exception;
}