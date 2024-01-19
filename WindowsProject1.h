#pragma once
#include "resource.h"
#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"
#include <atomic>
#include <exception>
#include "exportCSV.h"
#include <string>
#include <nidaqmx.h>
#include <iostream>
#include <string>
#include <vector>
#include "daq.h"
#include "brightnessCalculation.h"

void completeOfGraph(HWND pztGraphframe, std::deque<double> lineData, double maxVertical = 999.0) {
	HDC graphf = GetDC(pztGraphframe);
	RECT rect;
	GetClientRect(pztGraphframe, &rect);
	int padding = 2;
	rect.bottom = rect.bottom - padding;
	rect.right = rect.right - padding;
	rect.left = rect.left + padding;
	rect.top = rect.top + 9*padding;

	FillRect(graphf, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	double scaleY = 1.5;
	int startIndex = 0;
	int visibleDataPoints = rect.right;
	int endIndex = std::min(startIndex + visibleDataPoints, static_cast<int>(lineData.size()));

	if (lineData.size() >= static_cast<size_t>(visibleDataPoints - startIndex)) {
		lineData.erase(lineData.begin(), lineData.begin() + (lineData.size() - (visibleDataPoints - startIndex)));
	}
	for (int i = startIndex; i < endIndex - 1; ++i) {
		double y1 = lineData[i];
		double y2 = lineData[i + 1];
		double normalizedX1 = static_cast<double>(i - startIndex) / (visibleDataPoints - 1);
		double normalizedX2 = static_cast<double>(i + 1 - startIndex) / (visibleDataPoints - 1);

		int startX = static_cast<int>(normalizedX1 * rect.right);
		int endX = static_cast<int>(normalizedX2 * rect.right);

		// Scale the heights based on the available vertical space
		int startY = rect.bottom - static_cast<int>(y1 * scaleY * rect.bottom / maxVertical) - padding;
		int endY = rect.bottom - static_cast<int>(y2 * scaleY * rect.bottom / maxVertical)- padding;

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
}

class Camera {
public:
	int id = 0;
	cv::VideoCapture cap;
	int getId;
	bool stopCamera = true;
	std::deque<double> brightData, pztVolt;
	double brightness = 0;
	double pzt_vol = 0;
	bool startDepo = false;
	MyDaq daq;
	Camera() : getId(id) {

	}
	void setStopCamera(bool stop) {
		stopCamera = stop;
	}
	bool getstopCamera() const {
		return stopCamera;
	}
	void setDepositionBool(bool stop) {
		startDepo = stop;
	}
	bool getDepositionBool() const {
		return startDepo;
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
	std::deque<double>& GetPZTvolt() {
		return pztVolt;
	}
	std::deque<double>& GetBrightData() {
		return brightData;
	}
	void setBrightness(cv::Mat tmpcalcFrame) {
		BrightnessClass my = BrightnessClass(tmpcalcFrame);
		brightness = my.differencesOf();
	}
	void setPztVolt(double dddd) {
		pzt_vol = dddd;
	}
	const double getBrightness() const {
		return brightness;
	}
	const double getUpdateofPzt() const {
		return pzt_vol;
	}
	void DisplayCameraFrame(HWND hWnd, HWND hWn, HWND calframe)
	{
		cap.open(0);
		if (!cap.isOpened()) {
			return;
		}
		double newValue = 0;
		double maxvalue = 3;
		double steps = 1000;
		bool isIncrease = TRUE;
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
				if (getDepositionBool()) {
					if (newValue < maxvalue && isIncrease) {
						newValue += (maxvalue / steps);
					}
					if (newValue > maxvalue) {
						newValue = maxvalue;
						isIncrease = false;
					}
					if (newValue < 0) {
						newValue = 0;
					}
					if (newValue > 0 && !isIncrease) {
						newValue -= (maxvalue / steps);
					}
					setPztVolt(newValue);
					pztVolt.push_back(getUpdateofPzt());

				}
				if (!getDepositionBool()) {
					if (newValue < 0) {
						newValue = 0;
					}
					if (newValue > 0) {
						newValue -= (maxvalue / (steps * 0.25));
					}
					setPztVolt(newValue);
					pztVolt.push_back(getUpdateofPzt());
				}
				daq.addAnalogChannel("Dev2/ao0");
				daq.analogOut("Dev2/ao0", getUpdateofPzt());
				daq.startTasks();


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
