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
void Camera::cameraIndex() {
	PreferenceManager pref; // Assuming you have access to PreferenceManager

	std::string indexString = pref.getprefString(CameraIndex);


	std::string result;
	for (char c : indexString) {
		if (c == '\\') {
			result += "\\\\";
		}
		else if (c != '"') {
			result += c;
		}
	}

	if (indexString == "0") {
		cameraid = 0;
		filepath = result; // No filepath when camera index is 0
	}
	else if (indexString == "1") {
		cameraid = 1;
		filepath = result; // No filepath when camera index is 1
	}
	else {
		cameraid = -1; // Indicate invalid camera index
		filepath = result; // Set the filepath
	}
}
int Camera::getCameraId() const {
	return cameraid;
}

std::string Camera::getFilePath() const {
	return filepath;
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

void Camera::DisplayCameraFrame()
{
	OutputDebugStringW(L"\n\n\n\naayyo\n\n\n\n\n");
	

	try {
		sqw = std::stod(pref.getprefString(SQW_KEY));
		sqh = std::stod(pref.getprefString(SQH_KEY));
		sqx1 = std::stod(pref.getprefString(SQX1_KEY));
		sqy1 = std::stod(pref.getprefString(SQY1_KEY));
		cameraIndex();
		if (getCameraId() !=-1) {
			cap.open(getCameraId());
		}
		else {
			cap.open(getFilePath());
		}
		if (!cap.isOpened()) {
			return;
		}
		double stage = 0;
		
		const int camwidth = std::round(0.25 * GetSystemMetrics(SM_CXSCREEN));
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
		cv::Mat dframe, frame, tmpFrameOriginal, tmpFrameCropped, tmpcalcFrame, bbdd, ppttzz;
		cv::namedWindow("ORI", cv::WINDOW_AUTOSIZE);
		while (getstopCamera()) {
			uth = std::stod(pref.getprefString(UTH_KEY));
			lth = std::stod(pref.getprefString(LTH_KEY));
			pztmax = std::stod(pref.getprefString(PZT_KEY));
			cameraIndex();
			time = std::stod(pref.getprefString(TIME_KEY));
			cap >> dframe;
			if (!dframe.empty()) {
				cv::flip(dframe, frame, 1);
				cv::resize(frame, frame, cv::Size(camwidth - 10, rowheight - 10));
				cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);

				drawRectangle(tmpFrameOriginal,0, 20,125, 185 , cv::Scalar(0, 0, 255), 1);
				cv::Mat croppedFrame = frame(cv::Rect(0, 20, 125, 165));

				cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
				cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(camwidth - 10, rowheight - 10));


				drawRectangle(tmpFrameCropped, sqx1, sqy1, sqx1+sqw, sqy1+sqh, cv::Scalar(0, 0, 255), 1);
				cv::Mat calcFrame = tmpFrameCropped(cv::Rect(sqx1, sqy1, sqw, sqh));
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
							if (stage < (0.04 * pztmax)) {
								stage += (0.0002 * pztmax);
							}
							else {
								stage += pztmax / (time);
							}
							setEV(std::stod(pref.getprefString(EPV_KEY)));
						}
						if (isRedeposition && output) {
							stage += (pztmax / (time + timedelay));
							setEV(std::stod(pref.getprefString(EPV_KEY)));
						}
						if (!output) {
							timedelay += 1;
							stage -= pztmax / (time * 0.25);
							setEV(0);
							isRedeposition = true;
						}
						if (stage > pztmax && !isComplete) {
							isComplete = true;
							stage -= pztmax / time;
							setEV(0);
						}
						pztVolt.push_back(stage);

					}
					if (isComplete) {
						setEV(0);
						setCaptureScreenBool(TRUE);
						setDepositionBool(FALSE);
					}
					csv.saveCSV(brightData, pztVolt, current_filename);
				}
				if (!getDepositionBool()) {
					setEV(0);
					if (stage < 0) {
						stage = 0;
					}
					if (stage > 0) {
						stage -= (pztmax / (time * 0.25));
					}
					pztVolt.push_back(stage);
				}
				double hhee = getEV();
				DAQmxWriteAnalogF64(epvtask, 1, true, 10.0, DAQmx_Val_GroupByChannel, &hhee, nullptr, nullptr);
				DAQmxWriteAnalogF64(pztvtask, 1, true, 10.0, DAQmx_Val_GroupByChannel, &stage, nullptr, nullptr);

				// Display the original and cropped frames combined in a single window
				cv::Mat bd(200, 200, CV_8UC3, cv::Scalar(100, 100, 100));
				cv::cvtColor(bd, bbdd, cv::COLOR_BGR2BGRA);
				cv::resize(bbdd, bbdd, cv::Size(2 * tmpFrameOriginal.cols, 0.5*tmpFrameOriginal.rows));

				cv::Mat pztgraph(200, 200, CV_8UC3, cv::Scalar(100, 100, 100));
				cv::cvtColor(pztgraph, ppttzz, cv::COLOR_BGR2BGRA);
				cv::resize(ppttzz, ppttzz, cv::Size(2 * tmpFrameOriginal.cols, 0.5*tmpFrameOriginal.rows));

				allgraph(bbdd, brightData, 160, "BD");
				allgraph(ppttzz, pztVolt, 5, "PZT");

				cv::Mat combinedFrame(tmpFrameOriginal.rows * 2, tmpFrameOriginal.cols * 2, tmpFrameOriginal.type());
				tmpFrameOriginal.copyTo(combinedFrame(cv::Rect(0, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows)));
				tmpFrameCropped.copyTo(combinedFrame(cv::Rect(tmpFrameOriginal.cols, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows)));
				bbdd.copyTo(combinedFrame(cv::Rect(0, tmpFrameOriginal.rows, combinedFrame.cols, 0.5 * tmpFrameOriginal.rows)));
				ppttzz.copyTo(combinedFrame(cv::Rect(0, 1.5*tmpFrameOriginal.rows, combinedFrame.cols, 0.5 * tmpFrameOriginal.rows)));

				cv::imshow("ORI", combinedFrame);
				cv::moveWindow("ORI", -10, -10);
			}
			if (cv::waitKey(1) == 'q'|| !getstopCamera()) {
				stopCamera = true;
				cv::destroyWindow("ORI");
				break;
			}
		}
		DAQmxClearTask(epvtask);
		DAQmxClearTask(pztvtask);
		cap.release();
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

void Camera::allgraph(cv::Mat& frame, std::deque<double>& graphValues, double upperLimit, const std::string& yxix) {
	int startPointX = 30;
	if (graphValues.empty()) {
		return;
	}
	int height = frame.rows;
	int width = frame.cols;
	if (graphValues.size() >= static_cast<size_t>(width - startPointX)) {
		int elementsToSkip = graphValues.size() - (width - startPointX);
		startPointX -= elementsToSkip;
	}
	cv::Point startPoint(startPointX, height * 0.5);
	frame = cv::Scalar(255, 255, 255);
	line(frame, cv::Point(10, 10), cv::Point(width * 0.9, 10), cv::Scalar(0, 255, 0), 1);
	line(frame, cv::Point(10, height - 10), cv::Point(width * 0.9, height - 10), cv::Scalar(0, 0, 255), 1);
	for (int i = 0; i < graphValues.size(); ++i) {
		double y = (graphValues[i] / upperLimit) * (height * 0.8) + 10;
		cv::Point endPoint(i + startPointX, height - static_cast<int>(y));
		line(frame, startPoint, endPoint, cv::Scalar(0, 0, 0), 1);
		startPoint = endPoint;
	}
	if (getEV() == 0) {
		drawRectangle(frame, 0, 0, 10, 10, cv::Scalar(0, 0, 255), -1);
	}
	else {
		drawRectangle(frame, 0, 0, 10, 10, cv::Scalar(0,255,0), -1);
	}
	/*drawYAxisValues(frame, pr.uBGR(0, 0, 0), upperLimit, yxix);
	drawXAxis(frame, pr.uBGR(0, 0, 0));*/
}
