#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

class Task {
private:
    int id; // Task ID
  
public:
    Task(int taskId) : id(taskId) {}

    void execute() {
        std::cout << "Task " << id << " is starting." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
        std::cout << "Task " << id << " is completed." << std::endl;
    }
};


int main() {
    std::vector<std::thread> threads; // Vector to hold threads
    const int numTasks = 10; // Number of tasks

    // Create tasks and threads
    for (int i = 0; i < numTasks; ++i) {
        Task task(i + 1);
        threads.emplace_back(&Task::execute, task); // Start the task in a thread
    }

    // Join all threads to the main thread to ensure they complete execution
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}

