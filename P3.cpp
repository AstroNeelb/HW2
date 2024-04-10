#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

const int max_on_runway = 3;

class Aircraft {
public:
    explicit Aircraft(int id) : ac_id(id) {}

    void land() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate landing time
    }

    int get_id() const {
        return ac_id;
    }

private:
    int ac_id;
};

class ATC {
public:
    ATC() : runways_occupied_(0) {}

    void request_landing(Aircraft& ac) {
        std::unique_lock<std::mutex> lock(mutex_);

        std::cout << "Aircraft #" << ac.get_id() << " requesting landing.\n";

        // If the runway is full, redirect the aircraft.
        if (runways_occupied_ >= max_on_runway) {
            std::cout << "Runway full. Aircraft #" << ac.get_id() << " being redirected to another airport.\n";
        }
        else {
            // Otherwise, clear the aircraft to land
            std::cout << "Aircraft #" << ac.get_id() << " is cleared to land.\n";
            runways_occupied_++;
            lock.unlock(); // Unlock as the landing operation does not need to be protected

            ac.land(); // Simulate the landing

            lock.lock();
            runways_occupied_--;
            std::cout << "Aircraft #" << ac.get_id() << " has landed. Runway is now free.\n";
            cv_.notify_one(); // Notify one waiting aircraft that the runway is free
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int runways_occupied_;
};

int main() {
    ATC atc;
    std::vector<Aircraft> aircrafts;
    std::vector<std::thread> threads;

    for (int i = 1; i <= 10; ++i) {
        aircrafts.emplace_back(i);
    }

    // Create threads for each aircraft's landing request
    for (auto& ac : aircrafts) {
        threads.emplace_back([&atc, &ac] {
            atc.request_landing(ac);
        });
    }

    // Join threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}