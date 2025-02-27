#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

std::mutex mtx1; // First mutex
std::mutex mtx2; // Second mutex

void taskWithDeadlock(int taskId) {
      std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);

    // Lock both mutexes in a specific order
    std::lock(lock1, lock2); // Safely locks both mutexes

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
