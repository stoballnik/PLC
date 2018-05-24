#include <iostream>
#include <thread>

#include "Future.h"
#include "Promise.h"
#include "Async.h"

int main() {
    ThreadPool pool;
    std::function<int(int, int)> task = [](int time, int result) {
        std::this_thread::sleep_for(std::chrono::seconds(time));
        return result;
    };

    std::vector<Future<int>> tasks;
    for (int i = 0; i < 15; ++i) {
        tasks.push_back(Async(Method::Async, &pool, task, 1, i));
    }

    for (auto &value: tasks) {
        try {
            std::cout << *value.Get() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}