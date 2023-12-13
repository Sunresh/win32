#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>

class MasterThread {
public:
	MasterThread() : running_(false) {}

	void addTask(std::function<void()> task) {
		std::lock_guard<std::mutex> lock(mutex_);
		tasks_.emplace_back(task);
	}

	void start() {
		if (!running_) {
			running_ = true;
			masterThread_ = std::thread([this] { this->run(); });
		}
	}

	void stop() {
		if (running_) {
			running_ = false;
			if (masterThread_.joinable()) {
				masterThread_.join();
			}
		}
	}

	~MasterThread() {
		stop();
	}

private:
	std::vector<std::function<void()>> tasks_;
	std::thread masterThread_;
	bool running_;
	std::mutex mutex_;
	void run() {
		while (running_) {
			std::vector<std::function<void()>> currentTasks;
			{
				std::lock_guard<std::mutex> lock(mutex_);
				currentTasks = tasks_;
			}

			for (auto& task : currentTasks) {
				if (!running_) {
					break;
				}
				task(); // Execute the task
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small delay to yield
		}
	}
};


/*
// Example usage:
void task1() {
	// Your task implementation
	std::cout << "Task 1 is running..." << std::endl;
}

void task2() {
	// Your task implementation
	std::cout << "Task 2 is running..." << std::endl;
}

int main() {
	MasterThread masterThread;

	// Add tasks to the master thread
	masterThread.addTask(task1);
	masterThread.addTask(task2);

	// Start the master thread
	masterThread.start();

	// Simulate the program running for some time
	std::this_thread::sleep_for(std::chrono::seconds(5));

	// Stop the master thread when the program ends
	masterThread.stop();

	return 0;
}
*/