#pragma once

#include "SharedState.h"
#include <memory>
#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include <cassert>

template<class T>
class Promise {
public:
	void SetValue(T* newValue);
	void SetException(std::exception_ptr newException);

	Future<T> GetFuture();
	Promise() : data(new SharedState<T>()) {}

	friend class Future<T>;
private:
	std::shared_ptr<SharedState<T>> data;
};

template<class T>
void Promise<T>::SetValue(T* newValue) {
	data->SetValue(newValue);
}

template<class T>
void Promise<T>::SetException(std::exception_ptr newException) {
	data->SetException(newException);
}

template<class T>
Future<T> Promise<T>::GetFuture() {
	return Future<T>(data);
}