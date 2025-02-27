#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <queue>
#include <functional>

enum TaskStatus { PENDING, IN_PROGRESS, COMPLETED };

class Task {
private:
    int id;                     // Task ID
    int priority;               // Task priority 
    TaskStatus status;          // Status of the task
    static std::mutex mtx;     // Static mutex for resource protection

public:
    Task(int taskId, int taskPriority) : id(taskId), priority(taskPriority), status(PENDING) {}

    void execute() {
        {
            std::lock_guard<std::mutex> lock(mtx); // Lock for thread safety
            status = IN_PROGRESS;                   // Update task status
            std::cout << "Task " << id << " is starting." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work

        {
            std::lock_guard<std::mutex> lock(mtx); // Lock for thread safety
            status = COMPLETED;                     // Update task status
            std::cout << "Task " << id << " is completed." << std::endl;
        }
    }

    int getPriority() const { return priority; }
    TaskStatus getStatus() const { return status; }
};

// Define the static mutex outside the class
std::mutex Task::mtx; // Correctly defining the static mutex

// Custom comparator for priority queue
struct TaskComparator {
    bool operator()(const Task& t1, const Task& t2) {
        return t1.getPriority() > t2.getPriority(); // Lower priority number is higher priority
    }
};

int main() {
    std::priority_queue<Task, std::vector<Task>, TaskComparator> tasks; // Priority queue for tasks

    // Create 10 tasks with varying priorities
    for (int i = 1; i <= 10; ++i) {
        tasks.emplace(i, 11 - i); // Higher priority for lower IDs (1 is highest priority)
    }

    std::vector<std::thread> threads;

    // Execute tasks based on priority
    while (!tasks.empty()) {
        Task task = tasks.top(); // Get the task with the highest priority
        tasks.pop();             // Remove it from the queue

        threads.emplace_back(&Task::execute, task); // Start execution in a thread
    }

    // Join all threads to ensure they complete execution
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
