#pragma once
#include "resource.h"
#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"
#include <atomic>
#include <exception>

class Camera {
private:
	int id = 0;
	cv::VideoCapture cap;
	int getId;
	bool stopCamera = true;
	std::deque<double> brightData;
	double brightness = 0;

public:
	Camera() : getId(id) {

	}
	void setStopCamera(bool stop) {
		stopCamera = stop;
	}
	bool getstopCamera() const {
		return stopCamera;
	}
	void start() {
		OutputDebugStringW(L"\naayyo\n");
	}
	void drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness = 1) {
		cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
	}
	double meanofBri(cv::Mat& iframe) {
		cv::Mat grayFrame;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
		double width = grayFrame.cols;
		double height = grayFrame.rows;
		double variance = 0.0;
		double meanCV = cv::mean(grayFrame)[0];
		return meanCV;
	}
	std::deque<double>& GetBrightData(){
		return brightData;
	}
	void setBrightness(cv::Mat tmpcalcFrame) {
		brightness = meanofBri(tmpcalcFrame);
	}
	const double getBrightness() const{
		return brightness;
	}
	void DisplayCameraFrame(HWND hWnd, HWND hWn, HWND calframe)
	{
		cap.open(0);
		if (!cap.isOpened()) {
			return;
		}
		HDC hdc = GetDC(hWnd); // Obtain device context for the window
		HDC hdc1 = GetDC(hWn); // Obtain device context for the window
		HDC hdcCalframe = GetDC(calframe);
		cv::Mat dframe, frame;
		while (!getstopCamera()) {
			cap >> dframe;
			if (!dframe.empty()) {
				cv::flip(dframe, frame, 1);//flip
				cv::resize(frame, frame, cv::Size(260, 215));
				// Show original frame in g_hFrame1
				cv::Mat tmpFrameOriginal;
				cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);
				// Show cropped portion in zoomfram
				drawRectangle(tmpFrameOriginal, 50, 50, 150, 150, cv::Scalar(0, 0, 255), 1);
				cv::Mat croppedFrame = frame(cv::Rect(50, 50, 100, 100)); // Example crop - adjust as needed
				cv::Mat tmpFrameCropped;
				cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
				cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(260, 215));
				// Show calfram portion in zoomfram
				drawRectangle(tmpFrameCropped, 50, 50, 150, 150, cv::Scalar(0, 0, 255), 1);
				cv::Mat calcFrame = tmpFrameCropped(cv::Rect(50, 50, 100, 100)); // Example crop - adjust as needed
				cv::Mat tmpcalcFrame;
				cv::cvtColor(calcFrame, tmpcalcFrame, cv::COLOR_BGR2BGRA);
				cv::resize(tmpcalcFrame, tmpcalcFrame, cv::Size(260, 215));

				setBrightness(tmpcalcFrame);
				brightData.push_back(getBrightness());
//				std::wstring meanString = std::to_wstring(getBrightness());
//				OutputDebugStringW(meanString.c_str());

				// Display original frame in g_hFrame1
				HBITMAP hBitmapOriginal = CreateBitmap(tmpFrameOriginal.cols, tmpFrameOriginal.rows, 1, 32, tmpFrameOriginal.data);
				HDC memDCOriginal = CreateCompatibleDC(hdc);
				SelectObject(memDCOriginal, hBitmapOriginal);
				BitBlt(hdc, 0, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows, memDCOriginal, 0, 0, SRCCOPY);
				DeleteDC(memDCOriginal);
				DeleteObject(hBitmapOriginal);
				// Display cropped portion in zoomfram
				HBITMAP hBitmapCropped = CreateBitmap(tmpFrameCropped.cols, tmpFrameCropped.rows, 1, 32, tmpFrameCropped.data);
				HDC memDCCropped = CreateCompatibleDC(hdc1);
				SelectObject(memDCCropped, hBitmapCropped);
				BitBlt(hdc1, 0, 0, tmpFrameCropped.cols, tmpFrameCropped.rows, memDCCropped, 0, 0, SRCCOPY);
				DeleteDC(memDCCropped);
				DeleteObject(hBitmapCropped);
				// Display calframe portion in zoomfram
				HBITMAP hBitmapcalframe = CreateBitmap(tmpcalcFrame.cols, tmpcalcFrame.rows, 1, 32, tmpcalcFrame.data);
				HDC memDCCalcframe = CreateCompatibleDC(hdcCalframe);
				SelectObject(memDCCalcframe, hBitmapcalframe);
				BitBlt(hdcCalframe, 0, 0, tmpcalcFrame.cols, tmpcalcFrame.rows, memDCCalcframe, 0, 0, SRCCOPY);
				DeleteDC(memDCCalcframe);
				DeleteObject(hBitmapcalframe);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			if (cv::waitKey(1) == 'q') {
				stopCamera = true;
				break;
			}
		}
		cap.release();
		ReleaseDC(hWnd, hdc);
		ReleaseDC(hWn, hdc1);
		ReleaseDC(calframe, hdcCalframe);
	}


};

#include <nidaqmx.h>
#include <iostream>
#include <string>
#include <vector>

class MyDaq {
private:
	TaskHandle analogTask;
	TaskHandle digitalTask;

public:
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

class Deposition {
private:
	int mal = 0;
public:
	std::deque<double>PPzz;
	double pztMax = 3.0;
	double newValue = 0.0;
	double steps = 1000;
	bool isIncrease = true;
	bool isDeposition = true;
	MyDaq daq;
	void setIsdeposition(bool booly) {
		isDeposition = booly;
	}
	bool getIsDepo() {
		return isDeposition;
	}
	void depositionFunction(HWND pztGraphframe, HWND info) {
		while (getIsDepo()) {
			try {
				if (newValue < pztMax && isIncrease) {
					newValue += (pztMax / steps);
				}
				if (newValue > pztMax) {
					newValue = pztMax;
					isIncrease = false;
				}
				if (newValue < 0) {
					newValue = 0;
					isIncrease = true;
					return;
				}
				if (newValue > 0 && !isIncrease) {
					newValue -= (pztMax / steps);
				}
				HWND hWndLowerTh = GetDlgItem(info, IDC_YOUR_LOWER_TH_STATIC_ID);
				if (hWndLowerTh != NULL) {
					std::wstring newText = L"Lower Th.: " + std::to_wstring(newValue); // Atomic load operation
					SetWindowTextW(hWndLowerTh, newText.c_str());
				}
				PPzz.push_back(newValue);
				HDC graphf = GetDC(pztGraphframe);
				RECT rect;
				GetClientRect(pztGraphframe, &rect);
				FillRect(graphf, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
				double scaleY = 1;
				int startIndex = 0;
				int visibleDataPoints = rect.right;
				int endIndex = std::min(startIndex + visibleDataPoints, static_cast<int>(PPzz.size()));

				if (PPzz.size() >= static_cast<size_t>(visibleDataPoints - startIndex)) {
					PPzz.erase(PPzz.begin(), PPzz.begin() + (PPzz.size() - (visibleDataPoints - startIndex)));
				}
				for (int i = startIndex; i < endIndex - 1; ++i) {
					double y1 = PPzz[i];
					double y2 = PPzz[i + 1];
					double normalizedX1 = static_cast<double>(i - startIndex) / (visibleDataPoints - 1);
					double normalizedX2 = static_cast<double>(i + 1 - startIndex) / (visibleDataPoints - 1);

					int startX = static_cast<int>(normalizedX1 * rect.right);
					int endX = static_cast<int>(normalizedX2 * rect.right);

					// Scale the heights based on the available vertical space
					int startY = rect.bottom - static_cast<int>(y1 * scaleY * rect.bottom / pztMax);
					int endY = rect.bottom - static_cast<int>(y2 * scaleY * rect.bottom / pztMax);

					// Clip the coordinates to ensure they are within the visible area
					startX = std::max<int>(0, std::min<int>(rect.right, startX));
					endX = std::max<int>(0, std::min<int>(rect.right, endX));
					startY = std::max<int>(0, std::min<int>(rect.bottom, startY));
					endY = std::max<int>(0, std::min<int>(rect.bottom, endY));

					// Draw the line only if it's within the visible area
					if (startX < rect.right && endX > 0 && startY < rect.bottom && endY > 0) {
						MoveToEx(graphf, startX, startY, NULL);
						LineTo(graphf, endX, endY);
					}
				}

				startIndex++;
				ReleaseDC(pztGraphframe, graphf);

				daq.addAnalogChannel("Dev2/ao0");
				daq.analogOut("Dev2/ao0", newValue);
				daq.startTasks();

				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
			catch (const std::exception& e) {
				// Convert narrow string to wide string
				int bufferSize = MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, nullptr, 0);
				if (bufferSize > 0) {
					std::vector<wchar_t> wideBuffer(bufferSize);
					MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, wideBuffer.data(), bufferSize);

					// Output the wide string
					OutputDebugStringW(L"Exception occurred: ");
					OutputDebugStringW(wideBuffer.data());
					OutputDebugStringW(L"\n");
				}
			}
			catch (...) {
				OutputDebugStringW(L"Unknown exception occurred\n");
			}

		}
	}
};
