#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

const int numofrobots = 5;
const int numoftools = 5;
const int task_duration = 5;

std::vector<bool> tools(numoftools, true);
std::mutex mutex;

void performTask(int robotID, int left_tool_idx, int right_tool_idx) {
    bool left_tool_acquired = false;
    bool right_tool_acquired = false;

    while (!left_tool_acquired || !right_tool_acquired) {
        std::unique_lock<std::mutex> lock(mutex);

        if (tools[left_tool_idx] && tools[right_tool_idx]) {
            tools[left_tool_idx] = false;
            tools[right_tool_idx] = false;
            left_tool_acquired = true;
            right_tool_acquired = true;
            lock.unlock();

            std::cout << "Robot " << robotID << " is collecting data." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 1 second

            lock.lock();
            std::cout << "Robot " << robotID << " acquired tools and begins its task." << std::endl;
            lock.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(task_duration));
            lock.lock();
            std::cout << "Robot " << robotID << " completed the task and returns the tools." << std::endl;

            tools[left_tool_idx] = true;
            tools[right_tool_idx] = true;
        }
    }
}

int main() {
    // Start threads for each robot
    std::vector<std::thread> robots;
    auto start_time = std::chrono::steady_clock::now(); // Start time

    for (int i = 0; i < numofrobots; ++i) {
        int left_tool_idx = i;
        int right_tool_idx = (i + 1) % numoftools;
        robots.emplace_back(performTask, i, left_tool_idx, right_tool_idx);
    }

    // Join threads
    for (auto& robot : robots) {
        robot.join();
    }

    auto end_time = std::chrono::steady_clock::now(); // End time
    auto total_runtime = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::cout << "Total runtime of the program: " << total_runtime.count() << " seconds." << std::endl;

    return 0;
}