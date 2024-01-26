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

void Camera::setEV(double ephv) {
	epv = ephv;
}
double Camera::getEV() {
	return epv;
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
const double Camera::getBrightness(){
	return brightness;
}
void Camera::setSQW(double ss) {
	sqw = ss;
}
void Camera::pauseCamera() {
	setStopCamera(TRUE);
	cap.release();
	sqw = std::stod(pref.getprefString(SQW_KEY));
	sqw = std::stod(pref.getprefString(SQH_KEY));
}

void Camera::DisplayCameraFrame(HWND hWnd, HWND hWn)
{
	OutputDebugStringW(L"\n\n\n\naayyo\n\n\n\n\n");
	sqw = std::stod(pref.getprefString(SQW_KEY));
	sqh = std::stod(pref.getprefString(SQH_KEY));
	sqx1 = std::stod(pref.getprefString(SQX1_KEY));
	sqy1 = std::stod(pref.getprefString(SQY1_KEY));
	time = std::stod(pref.getprefString(TIME_KEY));
	uth = std::stod(pref.getprefString(UTH_KEY));
	lth = std::stod(pref.getprefString(LTH_KEY));
	epv = std::stod(pref.getprefString(EPV_KEY));
	pztmax = std::stod(pref.getprefString(PZT_KEY));

	try {
		cap.open(0);
		if (!cap.isOpened()) {
			return;
		}
		double stage = 0;
		
		const int camwidth = std::round(0.4 * GetSystemMetrics(SM_CXSCREEN));
		const int rowheight = std::round(0.45 * GetSystemMetrics(SM_CYSCREEN));
		double timedelay = 0;
		bool isComplete = FALSE;
		bool isRedeposition = FALSE;


		DAQmxCreateTask("", &epvtask);
		DAQmxCreateTask("", &pztvtask);
		DAQmxCreateAOVoltageChan(epvtask, "Dev2/ao0", "ao_channel", 0.0, 5.0, DAQmx_Val_Volts, nullptr);
		DAQmxCreateAOVoltageChan(pztvtask, "Dev2/ao1", "ao_channel", 0.0, 5.0, DAQmx_Val_Volts, nullptr);
		DAQmxCfgSampClkTiming(epvtask, "", 10.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1);
		DAQmxCfgSampClkTiming(pztvtask, "", 10.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1);


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
				drawRectangle(tmpFrameCropped, sqx1, sqy1, sqx1+sqw, sqy1+sqh, cv::Scalar(0, 0, 255), 1);
				cv::Mat calcFrame = tmpFrameCropped(cv::Rect(sqx1, sqy1, sqw, sqh)); // Example crop - adjust as needed
				cv::Mat tmpcalcFrame;
			
				cv::cvtColor(calcFrame, tmpcalcFrame, cv::COLOR_BGR2BGRA);

				setBrightness(tmpcalcFrame);
				brightData.push_back(getBrightness());
				bool output = pref.schmittTrigger(getBrightness(), uth, lth, false);
				if (getDepositionBool()) {
					current_filename = pref.getprefString(CURRENT_FILENAME_KEY);
					if (!isComplete) {
						if (stage < 0) {
							stage = 0;
							timedelay = 0.0;
						}
						if (!isRedeposition && output) {
							stage += (pztmax / time);
							epv = 2;
						}
						if (isRedeposition && output) {
							stage += (pztmax / (time + timedelay));
							epv = 2;
						}
						if (!output) {
							timedelay += 1;
							stage -= pztmax / (time * 0.25);
							epv = 2;
							isRedeposition = true;
						}
						if (stage > pztmax && !isComplete) {
							isComplete = true;
							stage -= pztmax / time;
							epv = 0;
						}
						pztVolt.push_back(stage);

					}
					if (isComplete) {
						epv = 0;
						setCaptureScreenBool(TRUE);
						setDepositionBool(FALSE);
					}
					csv.saveCSV(brightData, pztVolt, current_filename);
				}
				if (!getDepositionBool()) {
					epv = 0;
					if (stage < 0) {
						stage = 0;
					}
					if (stage > 0) {
						stage -= (pztmax / (time * 0.25));
					}
					pztVolt.push_back(stage);
				}
				DAQmxWriteAnalogF64(epvtask, 1, true, 10.0, DAQmx_Val_GroupByChannel, &epv, nullptr, nullptr);
				DAQmxWriteAnalogF64(pztvtask, 1, true, 10.0, DAQmx_Val_GroupByChannel, &stage, nullptr, nullptr);


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
		DAQmxClearTask(epvtask);
		DAQmxClearTask(pztvtask);
		cap.release();
		ReleaseDC(hWnd, hdc);
		ReleaseDC(hWn, hdc1);
	}
	catch (const std::exception& e) {
		// Handle exceptions and display an error message
		myUIInstance.messi(e.what());
	}
	catch (...) {
		// Handle other types of exceptions
		myUIInstance.mess(L"Unknown error occurred.");
	}
}

