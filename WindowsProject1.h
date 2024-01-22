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
#include <Windows.h>
#include <wingdi.h>
#include "dialog_cameraoption.h"
#include "preferencemanager.h"

PreferenceManager preferenceManager;
std::string loadedValue;
int getFullwidth() {
	return GetSystemMetrics(SM_CXSCREEN);
}
int getFullheight() {
	return GetSystemMetrics(SM_CYSCREEN);
}
HWND CreateButton(const wchar_t* text, int x, int y,int buttonWidth, int buttonHeight, HWND parent, int id, DWORD style = BS_PUSHBUTTON)
{
	return CreateWindowW(L"BUTTON", text, WS_VISIBLE | WS_CHILD | style, x, y, buttonWidth, buttonHeight, parent, (HMENU)id, NULL, NULL);
}

HWND CreateStaticText(const wchar_t* text, int x, int y, int width, int height, HWND parent, int id, DWORD style = WS_BORDER)
{
	return CreateWindowW(L"STATIC", text, WS_VISIBLE | WS_CHILD | style, x, y, width, height, parent, (HMENU)id, NULL, NULL);
}

void mess(const wchar_t* text = L"Message") {
	MessageBoxW(NULL, text, L"Message", MB_OK | MB_ICONINFORMATION);
}


// Function to save preferences to the registry
void SavePreferences(const wchar_t* keyName, const wchar_t* valueName, DWORD value) {
	HKEY hKey;
	if (RegCreateKeyExW(HKEY_CURRENT_USER, keyName, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
		RegSetValueExW(hKey, valueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(DWORD));
		RegCloseKey(hKey);
	}
}

// Function to load preferences from the registry
DWORD LoadPreferences(const wchar_t* keyName, const wchar_t* valueName, DWORD defaultValue) {
	DWORD value = defaultValue;
	HKEY hKey;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, keyName, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		DWORD dataSize = sizeof(DWORD);
		RegQueryValueExW(hKey, valueName, NULL, NULL, reinterpret_cast<BYTE*>(&value), &dataSize);
		RegCloseKey(hKey);
	}
	return value;
}

INT_PTR CALLBACK CustomDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE; // Focus is set to the edit control on dialog initialization

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) {
			// OK button clicked, retrieve text from the edit control
			wchar_t buffer[256];
			GetDlgItemTextW(hwndDlg, IDC_EDIT_INPUT, buffer, sizeof(buffer) / sizeof(buffer[0]));

			// Process the input as needed
			MessageBoxW(NULL, buffer, L"User Input", MB_OK | MB_ICONINFORMATION);

			// Close the dialog
			EndDialog(hwndDlg, IDOK);
		}
		else if (LOWORD(wParam) == IDCANCEL) {
			// Cancel button clicked
			EndDialog(hwndDlg, IDCANCEL);
		}
		return TRUE; // Message handled
	}
	return FALSE; // Message not handled
}

void CaptureAndSaveScreenshot(const wchar_t* filePath)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd;
	SavePreferences(L"Software\\YourAppName", L"WindowSize", 800);
	DWORD loadedValue = LoadPreferences(L"Software\\YourAppName", L"WindowSize", 600);

	wchar_t loadedValueString[20]; // Adjust the buffer size as needed
	swprintf(loadedValueString, L"%u", loadedValue);

	//mess(loadedValueString);
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CUSTOM_DIALOG), NULL, CustomDialogProc);
	// Output to debug

	HDC hdcScreen = GetDC(NULL);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
	SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = screenWidth;
	bmpInfo.bmiHeader.biHeight = -screenHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	DWORD dataSize;
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, nullptr, &bmpInfo, DIB_RGB_COLORS);
	dataSize = bmpInfo.bmiHeader.biSizeImage;

	BYTE* buffer = new BYTE[dataSize];
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, buffer, &bmpInfo, DIB_RGB_COLORS);

	// Add BMP file header
	BITMAPFILEHEADER bmpFileHeader;
	bmpFileHeader.bfType = 0x4D42;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + bmpInfo.bmiHeader.biSize + dataSize;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + bmpInfo.bmiHeader.biSize;

	HANDLE hFile = CreateFileW(filePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;
		WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &bytesWritten, nullptr);
		WriteFile(hFile, &bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &bytesWritten, nullptr);
		WriteFile(hFile, buffer, dataSize, &bytesWritten, nullptr);
		CloseHandle(hFile);
	}

	delete[] buffer;
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(NULL, hdcScreen);
}

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
	void DisplayCameraFrame(HWND hWnd, HWND hWn)
	{
		cap.open(0);
		if (!cap.isOpened()) {
			return;
		}
		double newValue = 0;
		double maxvalue = 3;
		double steps = 1000;
		bool isIncrease = TRUE;
		const int camwidth = std::round(0.4 * getFullwidth());
		const int rowheight = std::round(0.45 * getFullheight());

		HDC hdc = GetDC(hWnd); // Obtain device context for the window
		HDC hdc1 = GetDC(hWn); // Obtain device context for the window
		cv::Mat dframe, frame;
		while (!getstopCamera()) {
			cap >> dframe;
			if (!dframe.empty()) {
				cv::flip(dframe, frame, 1);//flip
				cv::resize(frame, frame, cv::Size(camwidth-10, rowheight-10));
				// Show original frame in g_hFrame1
				cv::Mat tmpFrameOriginal;
				cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);
				// Show cropped portion in zoomfram
				drawRectangle(tmpFrameOriginal, (camwidth - 10) / 3, (rowheight - 10) / 3, 2*(camwidth - 10) / 3, 2*(rowheight - 10) / 3, cv::Scalar(0, 0, 255), 1);
				cv::Mat croppedFrame = frame(cv::Rect((camwidth - 10) / 3, (rowheight - 10) / 3, (camwidth - 10) / 3, (rowheight - 10) / 3)); 
				cv::Mat tmpFrameCropped;
				cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
				cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(camwidth-10, rowheight-10));
				// Show calfram portion in zoomfram
				drawRectangle(tmpFrameCropped, (camwidth - 10)*0.45, (rowheight - 10)*0.45, (camwidth - 10)*0.55, (rowheight - 10)*0.55, cv::Scalar(0, 0, 255), 1);
				cv::Mat calcFrame = tmpFrameCropped(cv::Rect((camwidth - 10) * 0.45, (rowheight - 10) * 0.45, (camwidth - 10) * 0.1, (rowheight - 10) * 0.1)); // Example crop - adjust as needed
				cv::Mat tmpcalcFrame;
				cv::cvtColor(calcFrame, tmpcalcFrame, cv::COLOR_BGR2BGRA);

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
	}


};
