#ifndef DAQ_H
#define DAQ_H

#include <nidaqmx.h>
#include <iostream>

class MyDaq {
public:
	TaskHandle analogTask;
	TaskHandle digitalTask;
	MyDaq() : analogTask(nullptr), digitalTask(nullptr) {
		// Create tasks for analog and digital outputs
		DAQmxCreateTask("AnalogTask", &analogTask);
		DAQmxCreateTask("DigitalTask", &digitalTask);
	}

	~MyDaq() {
		// Clear tasks on destruction
		if (analogTask != nullptr) {
			DAQmxStopTask(analogTask);
			DAQmxClearTask(analogTask);
		}
		if (digitalTask != nullptr) {
			DAQmxStopTask(digitalTask);
			DAQmxClearTask(digitalTask);
		}
	}

	void addAnalogChannel(const char* channel) {
		DAQmxCreateAOVoltageChan(analogTask, channel, "", 0.0, 5.0, DAQmx_Val_Volts, nullptr);
	}

	void addDigitalChannel(const char* channel) {
		DAQmxCreateDOChan(digitalTask, channel, "", DAQmx_Val_ChanForAllLines);
	}

	void startTasks() {
		DAQmxStartTask(analogTask);
		DAQmxStartTask(digitalTask);
	}

	void analogOut(const char* channel, double voltage) {
		uInt32 writtenSamples;
		DAQmxWriteAnalogScalarF64(analogTask, true, 10.0, voltage, nullptr);
	}

	void digitalOut(const char* channel, bool value) {
		uInt32 data = value ? 1 : 0;
		DAQmxWriteDigitalScalarU32(digitalTask, true, 10.0, data, nullptr);
	}

	void handleError(int32 error) {
		if (DAQmxFailed(error)) {
			char errBuff[2048] = { '\0' };
			DAQmxGetExtendedErrorInfo(errBuff, 2048);

			std::cerr << "DAQmx Error: " << errBuff << std::endl;
		}
	}
};


#endif // !DAQ_H
