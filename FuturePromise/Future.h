#pragma once

#include "SharedState.h"
#include "Promise.h"
#include <memory>
#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <cassert>

template<class T>
class Future {
public:
	Future() {};

    std::shared_ptr<T> Get() const;
    std::shared_ptr<T> TryGet() const;

    void SetInitializer(std::function<void()> init);
    friend class Promise<T>;
    operator bool();

private:
    Future(std::shared_ptr<SharedState<T>> data) : data(data) {}

    std::shared_ptr<SharedState<T>> data;
    std::function<void()> initializer;
};


template<class T>
std::shared_ptr<T> Future<T>::Get() const {
    assert(data);
    if (initializer)
        initializer();

    std::unique_lock<std::mutex> lock(data->mtx);

    data->cv.wait(lock, [this]() {
        return !data->Empty();
    });
    

    if (data->GetException()) {
        throw data->GetException();
    }
    return data->GetValue();
}

template<class T>
std::shared_ptr<T> Future<T>::TryGet() const {
    return data->GetValue();
}

template<class T>
Future<T>::operator bool() {
    return data != nullptr;
}

template<class T>
void Future<T>::SetInitializer(std::function<void()> init) {
	initializer = init;
}
