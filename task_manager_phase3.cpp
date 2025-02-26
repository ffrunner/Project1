#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex mtx1; // First mutex
std::mutex mtx2; // Second mutex

void taskWithDeadlock(int taskId) {
    // Try to lock mtx1 first, then mtx2
    std::lock_guard<std::mutex> lock1(mtx1); // Acquire first mutex
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate some work

    // Then, attempt to lock the second mutex, which could lead to deadlock if another thread has it
    std::lock_guard<std::mutex> lock2(mtx2); // Attempt to acquire the second mutex

    std::cout << "Task " << taskId << " is executing." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate task work
    std::cout << "Task " << taskId << " has completed." << std::endl;
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(taskWithDeadlock, i + 1);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
