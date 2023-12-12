
#include <thread>
#include <chrono>
#include "daqSignal.h"
#include "framework.h"
#include "WindowsProject1.h"

class Deposition {
private:
	double pzt = 0;
	double maxPzt = 4.0;
	bool isRunning = false;
	std::thread depoThread;
	MyDaq daq;
public:
	void setStopDeposition(bool stop) {
		isRunning = stop;
	}
	bool getStopDeposition() const {
		return isRunning;
	}
	const double getPZT() const {
		return pzt;
	}

	void start() {
		if (!getStopDeposition()) {
			setStopDeposition(true);
			depoThread = std::thread([this]() {
				while (getStopDeposition()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust delay if needed

					if (pzt < maxPzt) {
						pzt += maxPzt / 100;
					}
					else {
						// Decrease until it reaches 0 or the lowest value
						while (pzt > 0) {
							pzt -= maxPzt / 100;
							std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Adjust delay if needed
						}
						setStopDeposition(false); // Stop the thread
					}
				}
				daq.start(nullptr, "Dev2/ao0",pzt);
				});
		}
	}

	void stop() {
		if (getStopDeposition()) {
			setStopDeposition(false);
			if (depoThread.joinable()) {
				depoThread.join();
			}
		}
	}
};
