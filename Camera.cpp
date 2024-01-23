#include "Camera.h"

void Camera::setStopCamera(bool stop) {
	stopCamera = stop;
}
bool Camera::getstopCamera(){
	return stopCamera;
}
void Camera::setDepositionBool(bool stop) {
	startDepo = stop;
}
bool Camera::getDepositionBool() {
	return startDepo;
}
void Camera::setCaptureScreenBool(bool stop) {
	iscapture = stop;
}
bool Camera::getCaptureScreenBool() {
	return iscapture;
}
void Camera::start() {
	OutputDebugStringW(L"\naayyo\n");
}
void Camera::drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness = 1) {
	cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
}
double Camera::meanofBri(cv::Mat& iframe) {
	cv::Mat grayFrame;
	cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
	double width = grayFrame.cols;
	double height = grayFrame.rows;
	double variance = 0.0;
	double meanCV = cv::mean(grayFrame)[0];
	return meanCV;
}
std::deque<double>& Camera::GetPZTvolt() {
	return pztVolt;
}
std::deque<double>& Camera::GetBrightData() {
	return brightData;
}
void Camera::setBrightness(cv::Mat& tmpcalcFrame) {
	brightness = my.differencesOf(tmpcalcFrame);
}
void Camera::setPztVolt(double dddd) {
	pzt_vol = dddd;
}
const double Camera::getBrightness(){
	return brightness;
}
const double Camera::getUpdateofPzt(){
	return pzt_vol;
}
void Camera::DisplayCameraFrame(HWND hWnd, HWND hWn)
{
	cap.open(0);
	if (!cap.isOpened()) {
		return;
	}
	double newValue = 0;
	double maxvalue = 3;
	double steps = 100;
	bool isIncrease = TRUE;
	const int camwidth = std::round(0.4 * GetSystemMetrics(SM_CXSCREEN));
	const int rowheight = std::round(0.45 * GetSystemMetrics(SM_CYSCREEN));

	HDC hdc = GetDC(hWnd); // Obtain device context for the window
	HDC hdc1 = GetDC(hWn); // Obtain device context for the window
	cv::Mat dframe, frame;
	while (!getstopCamera()) {
		cap >> dframe;
		if (!dframe.empty()) {
			cv::flip(dframe, frame, 1);//flip
			cv::resize(frame, frame, cv::Size(camwidth - 10, rowheight - 10));
			// Show original frame in g_hFrame1
			cv::Mat tmpFrameOriginal;
			cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);
			// Show cropped portion in zoomfram
			drawRectangle(tmpFrameOriginal, (camwidth - 10) / 3, (rowheight - 10) / 3, 2 * (camwidth - 10) / 3, 2 * (rowheight - 10) / 3, cv::Scalar(0, 0, 255), 1);
			cv::Mat croppedFrame = frame(cv::Rect((camwidth - 10) / 3, (rowheight - 10) / 3, (camwidth - 10) / 3, (rowheight - 10) / 3));
			cv::Mat tmpFrameCropped;
			cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
			cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(camwidth - 10, rowheight - 10));
			// Show calfram portion in zoomfram
			drawRectangle(tmpFrameCropped, (camwidth - 10) * 0.45, (rowheight - 10) * 0.45, (camwidth - 10) * 0.55, (rowheight - 10) * 0.55, cv::Scalar(0, 0, 255), 1);
			cv::Mat calcFrame = tmpFrameCropped(cv::Rect((camwidth - 10) * 0.45, (rowheight - 10) * 0.45, (camwidth - 10) * 0.1, (rowheight - 10) * 0.1)); // Example crop - adjust as needed
			cv::Mat tmpcalcFrame;
			cv::cvtColor(calcFrame, tmpcalcFrame, cv::COLOR_BGR2BGRA);

			setBrightness(tmpcalcFrame);
			brightData.push_back(getBrightness());
			bool output = pref.schmittTrigger(getBrightness(),10,0,false);
			if (getDepositionBool()) {
				if (newValue < maxvalue && isIncrease && output) {
					newValue += (maxvalue / steps);
				}
				if (newValue > maxvalue) {
					setCaptureScreenBool(TRUE);
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

