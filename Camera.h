#ifndef CAMERA_H
#define CAMERA_H
#include "opencv2/opencv.hpp"
#include <NIDAQmx.h>
#include <Windows.h>
#include "BrightnessCalculation.h"
#include "preferencemanager.h"
#include "MyUi.h"
#include "ExportCsv.h"
#include <opencv2/videoio.hpp>
#include <d2d1.h>
#include <d2d1helper.h>

class Camera {
public:
	Camera() {
		OutputDebugStringW(L"\n\nStarting Camera class\n\n");
	}
	~Camera() {
		OutputDebugStringW(L"\n\nStopping Camera class\n\n");
	}
	int id = 0;
	cv::VideoCapture cap;
	int getId;
	bool stopCamera = true;
	std::deque<double> brightData, pztVolt, sdValues;
	double brightness = 0;
	bool startDepo = false;
	TaskHandle epvtask = nullptr;
	TaskHandle pztvtask = nullptr;
	double sqw = 15;
	double sqh = 15;
	double sqx1 = 15;
	double sqy1 = 15;
	double msqx1 = 0;
	double msqy1 = 0;
	double msqx2 = 100;
	double msqy2 = 100;
	double time = 15;
	double uth = 15;
	double lth = 15;
	double pztmax = 15;
	double epv = 2;
	double kk = 15;
	ExportCSV csv;
	BrightnessClass my;
	MyUI myUIInstance;
	double uplimit = 1;
	bool cvflip = TRUE;
	bool iscapture = FALSE;

	PreferenceManager pref;
	int cameraid = 0;
	std::string filepath;
	cv::Rect box; // Rectangle to be resized
	bool drawing_box = false;
	
	cv::Rect boxSecond; // Rectangle to be resized
	bool drawing_boxSecond = false;
	std::string current_filename ="ExampleND";
	

	void Camera::setStopCamera(bool stop) {
		stopCamera = stop;
	}
	bool Camera::getcvflip() {
		return cvflip;
	}
	void Camera::setcvflip(bool stop) {
		cvflip = stop;
	}
	bool Camera::getstopCamera() {
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

	void Camera::setUpperlimit(double upli) {
		uplimit = upli;
	}
	double Camera::getUpperlimit() {
		return uplimit;
	}


	std::string Camera::getFilePath() const {
		return filepath;
	}
	std::deque<double>& Camera::GetBrightData() {
		return brightData;
	}
	std::deque<double>& Camera::finalData4graph() {
		return sdValues;
	}
	double Camera::feedbackSD() {
		if (pref.getprefString(ADORDIFF_KEY) == "on") {
			return delta(getBrightness());
		}
		else {
			return 10 * stdev(getBrightness());
		}
	}

	void Camera::setBrightness(cv::Mat& tmpcalcFrame) {
		double averageBrightness, diff, norm, sizze;
		my.differencesOf(tmpcalcFrame, averageBrightness, diff, norm, sizze);
		brightness = averageBrightness;
	}
	const double Camera::getBrightness() {
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

	void Camera::DisplayCameraFrame(HWND shoo)
	{
		try {
			sqw = std::stod(pref.getprefString(SQW_KEY));
			sqh = std::stod(pref.getprefString(SQH_KEY));
			sqx1 = std::stod(pref.getprefString(SQX1_KEY));
			sqy1 = std::stod(pref.getprefString(SQY1_KEY));
			msqx1 = std::stod(pref.getprefString(MSQX1_KEY));
			msqy1 = std::stod(pref.getprefString(MSQY1_KEY));
			msqx2 = std::stod(pref.getprefString(MSQX2_KEY));
			msqy2 = std::stod(pref.getprefString(MSQY2_KEY));


			cameraIndex();
			if (getCameraId() != -1) {
				cap.open(getCameraId());
				setcvflip(TRUE);
			}
			else {
				cap.open(getFilePath());
				setcvflip(FALSE);
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
			bool isWithoutredeposition = TRUE;
			bool isBasecomplte = FALSE;

			DAQmxCreateTask("", &epvtask);
			DAQmxCreateTask("", &pztvtask);
			DAQmxCreateAOVoltageChan(epvtask, "Dev2/ao0", "ao_channel", 0.0, 5.0, DAQmx_Val_Volts, nullptr);
			DAQmxCreateAOVoltageChan(pztvtask, "Dev2/ao1", "ao_channel", 0.0, 5.0, DAQmx_Val_Volts, nullptr);
			DAQmxCfgSampClkTiming(epvtask, "", 10.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1);
			DAQmxCfgSampClkTiming(pztvtask, "", 10.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1);
			cv::Mat dframe, frame, tmpFrameOriginal, tmpFrameCropped, tmpcalcFrame, bbdd, ppttzz, combinedFrame, sdgrphlayout;
			Camera dep;

			cv::VideoWriter videoWriter;
			bool isRecord = false;
			bool isEditMode = false;
			if (pref.getprefString(AUTORECORD_KEY) == "on" || getDepositionBool()) {
				videoWriter.open(pref.getprefString(CURRENT_FOLDER) + "\\" + pref.getprefString(CURRENT_FILENAME_KEY) + ".mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), 30.0, cv::Size(2 * (camwidth - 10), 2 * (rowheight - 10)));
				if (!videoWriter.isOpened()) {
					std::cerr << "Error: Couldn't open the output video file for writing!" << std::endl;
					return;
				}
			}
			if (pref.getprefString(EBOXONOFF_KEY) == "on") {
				cv::namedWindow("ORI", cv::WINDOW_AUTOSIZE);
				cv::setMouseCallback("ORI", &Camera::mouse_callback, &dep);
			}
			while (getstopCamera()) {
				uth = std::stod(pref.getprefString(UTH_KEY));
				lth = std::stod(pref.getprefString(LTH_KEY));
				pztmax = std::stod(pref.getprefString(PZT_KEY));
				cameraIndex();
				time = std::stod(pref.getprefString(TIME_KEY));
				current_filename = pref.getprefString(CURRENT_FILENAME_KEY);
				cap >> dframe;
				videoWriter << dframe;
				if (!dframe.empty()) {
					if (getcvflip()) {
						cv::flip(dframe, frame, 1);
					}
					else {
						frame = dframe.clone();
					}
					cv::resize(frame, frame, cv::Size(camwidth - 10, rowheight - 10));
					cv::cvtColor(frame, tmpFrameOriginal, cv::COLOR_BGR2BGRA);

					drawRectangle(tmpFrameOriginal, msqx1, msqy1, msqx1 + msqx2, msqy1 + msqy2, cv::Scalar(255, 255, 0), 1);
					cv::Mat croppedFrame = frame(cv::Rect(msqx1, msqy1, msqx2, msqy2));

					cv::cvtColor(croppedFrame, tmpFrameCropped, cv::COLOR_BGR2BGRA);
					cv::resize(tmpFrameCropped, tmpFrameCropped, cv::Size(camwidth - 10, rowheight - 10));

					drawRectangle(tmpFrameCropped, sqx1, sqy1, sqx1 + sqw, sqy1 + sqh, cv::Scalar(255, 0, 255), 1);
					cv::Mat calcFrame = tmpFrameCropped(cv::Rect(sqx1, sqy1, sqw, sqh));
					cv::cvtColor(calcFrame, tmpcalcFrame, cv::COLOR_BGR2BGRA);


					setBrightness(tmpcalcFrame);
					brightData.push_back(getBrightness());
					sdValues.push_back(feedbackSD());
					SchmittTrigger naresh(uth, lth);
					bool output = naresh.processInput(feedbackSD());

					if (getDepositionBool()) {
						if (stage < (0.04 * pztmax) && !isBasecomplte) {//Making Base
							stage += (0.25 * pztmax / time);
						}
						if (stage > (0.04 * pztmax)) {//after completing base
							isBasecomplte = TRUE;
						}
						if (stage < 0) {
							stage = 0;
							timedelay = 0.0;
						}
						if (!isRedeposition && output && isWithoutredeposition && isBasecomplte) {//normal deposition
							stage += pztmax / (time);
						}
						if (isRedeposition && output) {// Redeposition
							stage += (pztmax / (time + timedelay));
						}
						if (!output && isBasecomplte) { //stage control to get lost position
							timedelay += 1;
							stage -= pztmax / (time * 0.25);
							isRedeposition = true;
							isWithoutredeposition = false;
						}
						if (stage >= pztmax) {
							setCaptureScreenBool(TRUE);
							setDepositionBool(FALSE);
						}
						pztVolt.push_back(stage);
						setEV(std::stod(pref.getprefString(EPV_KEY)));
						
						if (!isRecord) {
							videoWriter.open(pref.getprefString(CURRENT_VIDEO_FOLDER) + "\\" + pref.getprefString(CURRENT_FILENAME_KEY) + ".mp4", cv::VideoWriter::fourcc('H', '2', '6', '4'), 30.0, cv::Size(2 * (camwidth - 10), 2 * (rowheight - 10)));
							isRecord = true;
						}
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
					cv::resize(bbdd, bbdd, cv::Size(2 * tmpFrameOriginal.cols, 0.33 * tmpFrameOriginal.rows));


					cv::Mat sdgraph(200, 200, CV_8UC3, cv::Scalar(100, 100, 100));
					cv::cvtColor(sdgraph, sdgrphlayout, cv::COLOR_BGR2BGRA);
					cv::resize(sdgrphlayout, sdgrphlayout, cv::Size(2 * tmpFrameOriginal.cols, 0.33 * tmpFrameOriginal.rows));


					cv::Mat pztgraph(200, 200, CV_8UC3, cv::Scalar(100, 100, 100));
					cv::cvtColor(pztgraph, ppttzz, cv::COLOR_BGR2BGRA);
					cv::resize(ppttzz, ppttzz, cv::Size(2 * tmpFrameOriginal.cols, 0.33 * tmpFrameOriginal.rows));


					allgraph(bbdd, GetBrightData(), 1, "Bri");
					allgraph(sdgrphlayout, finalData4graph(), 4, "SD");
					allgraph(ppttzz, pztVolt, std::stod(pref.getprefString(PZT_KEY)), "PZT");

					if (dep.drawing_box) {
						cv::rectangle(tmpFrameCropped, dep.box, cv::Scalar(255, 0, 0), 1);
						pref.SetPreference(SQX1_KEY, std::to_string((dep.box.x)));
						pref.SetPreference(SQY1_KEY, std::to_string(dep.box.y));
						pref.SetPreference(SQW_KEY, std::to_string(dep.box.width));
						pref.SetPreference(SQH_KEY, std::to_string(dep.box.height));
					}
					if (dep.drawing_boxSecond) {
						cv::rectangle(tmpFrameOriginal, dep.boxSecond, cv::Scalar(2, 0, 20), 1);
						pref.SetPreference(MSQX1_KEY, std::to_string(dep.boxSecond.x));
						pref.SetPreference(MSQY1_KEY, std::to_string(dep.boxSecond.y));
						pref.SetPreference(MSQX2_KEY, std::to_string(dep.boxSecond.width));
						pref.SetPreference(MSQY2_KEY, std::to_string(dep.boxSecond.height));
					}

					if (getEV() == 0) {
						drawRectangle(ppttzz, 0, ppttzz.rows - 20, 20, ppttzz.rows, cv::Scalar(0, 0, 255), -1);
					}
					else {
						drawRectangle(ppttzz, 0, ppttzz.rows - 20, 20, ppttzz.rows, cv::Scalar(0, 255, 0), -1);
					}
					cv::Mat combinedFrame(tmpFrameOriginal.rows * 2, tmpFrameOriginal.cols * 2, tmpFrameOriginal.type());
					tmpFrameOriginal.copyTo(combinedFrame(cv::Rect(0, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows)));
					tmpFrameCropped.copyTo(combinedFrame(cv::Rect(tmpFrameOriginal.cols, 0, tmpFrameOriginal.cols, tmpFrameOriginal.rows)));
					bbdd.copyTo(combinedFrame(cv::Rect(0, tmpFrameOriginal.rows, combinedFrame.cols, 0.33 * tmpFrameOriginal.rows)));
					sdgrphlayout.copyTo(combinedFrame(cv::Rect(0, 1.33 * tmpFrameOriginal.rows, combinedFrame.cols, 0.33 * tmpFrameOriginal.rows)));
					ppttzz.copyTo(combinedFrame(cv::Rect(0, 1.66 * tmpFrameOriginal.rows, combinedFrame.cols, 0.33 * tmpFrameOriginal.rows)));

					if (pref.getprefString(AUTOGRAPH_KEY) == "on" || getDepositionBool()) {
						csv.saveCSV(GetBrightData(),finalData4graph(), pztVolt, current_filename);
					}
					if (pref.getprefString(AUTORECORD_KEY) == "on" || getDepositionBool()) {
						videoWriter.write(combinedFrame);
					}
					if (!isEditMode && pref.getprefString(EBOXONOFF_KEY) == "on") {
						cv::namedWindow("ORI", cv::WINDOW_AUTOSIZE);
						cv::setMouseCallback("ORI", &Camera::mouse_callback, &dep);
						isEditMode = true;
					}
					if (pref.getprefString(EBOXONOFF_KEY) == "on") {
						showWindow("ORI", combinedFrame);
					}
					else {
						hideWindow("ORI");
					}
					

					RenderCombinedFrame(shoo,combinedFrame);
				}
				if (cv::waitKey(1) == 'q' || !getstopCamera()) {
					stopCamera = true;
					cv::destroyWindow("ORI");
					break;
				}
			}
			DAQmxClearTask(epvtask);
			DAQmxClearTask(pztvtask);
			videoWriter.release();
			cap.release();
		}
		catch (const std::exception& e) {
			myUIInstance.messi(e.what());
		}
		catch (...) {
			myUIInstance.mess(L"Unknown error occurred.");
		}
	}
	void hideWindow(const std::string& winName) {
		cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
		cv::setWindowProperty(winName, cv::WND_PROP_AUTOSIZE, cv::WINDOW_NORMAL);
		cv::setWindowProperty(winName, cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
		cv::setWindowProperty(winName, cv::WND_PROP_VISIBLE, cv::WINDOW_FULLSCREEN);
		cv::resizeWindow(winName, 1, 1);
	}
	void showWindow(const std::string& winName, cv::Mat& dframe) {
		cv::imshow(winName, dframe);
		cv::setWindowProperty(winName, cv::WND_PROP_AUTOSIZE, cv::WINDOW_AUTOSIZE);
		cv::moveWindow(winName, -10, -10);
	}

	static void Camera::mouse_callback(int event, int x, int y, int flags, void* param) {
		auto self = static_cast<Camera*>(param);
		int w = std::round(0.25 * GetSystemMetrics(SM_CXSCREEN) - 10);
		int h = std::round(0.45 * GetSystemMetrics(SM_CYSCREEN) - 10);
		PreferenceManager pref;
		switch (event) {
		case cv::EVENT_MOUSEMOVE://x2y2
			if (self->drawing_box) {
				if (x >= w && y >= 0 && y < h) {
					self->box.width = x - w - self->box.x;
					self->box.height = y - self->box.y;
				}
			}
			if (self->drawing_boxSecond) {
				if (x >= 0 && y >= 0 && x < w && y < h) {
					self->boxSecond.width = x - self->boxSecond.x;
					self->boxSecond.height = y - self->boxSecond.y;
				}
			}
			break;
		case cv::EVENT_LBUTTONDOWN://x1y1
			if (x >= w && y >= 0 && y < h) {
				self->drawing_box = true;
				self->box = cv::Rect(x - w, y, 0, 0);
			}
			if (x >= 0 && y >= 0 && x < w && y < h) {
				self->drawing_boxSecond = true;
				self->boxSecond = cv::Rect(x, y, 0, 0);
			}
			break;
		case cv::EVENT_LBUTTONUP:
			self->drawing_box = false;
			self->drawing_boxSecond = false;
			break;
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
		std::string cu;
		cv::Point startPoint(startPointX, height * 0.5);
		frame = cv::Scalar(255, 255, 255);
		line(frame, cv::Point(10, 10), cv::Point(width * 0.9, 10), cv::Scalar(0, 255, 0), 1);
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << upperLimit / 2; // Format double with 2 decimal places
		std::string myString = oss.str();
		cv::putText(frame, myString, cv::Point(0, height * 0.5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(10, 10, 10), 1);
		line(frame, cv::Point(10, height - 10), cv::Point(width * 0.9, height - 10), cv::Scalar(0, 0, 255), 1);
		for (int i = 0; i < graphValues.size(); ++i) {
			cu = std::to_string(graphValues[i]);
			double y = (graphValues[i] / upperLimit) * (height * 0.8) + 10;
			cv::Point endPoint(i + startPointX, height - static_cast<int>(y));
			line(frame, startPoint, endPoint, cv::Scalar(0, 0, 0), 1);
			startPoint = endPoint;
		}
		cv::putText(frame, cu, cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 10, 10), 1);
		cv::putText(frame, pref.getprefString(CURRENT_FILENAME_KEY), cv::Point(width/2, 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 10, 10), 1);

	}


	double Camera::delta(double contrast) {
		static std::deque<double> contrastValues; // Static to retain values between function calls
		double sum = 0;
		int expectedSize = 30;

		contrastValues.push_back(contrast);
		while (contrastValues.size() > expectedSize) {
			contrastValues.pop_front();
		}
		if (contrastValues.size() > 1) {
			auto it = contrastValues.begin();
			double prevVal = *it;
			++it;
			while (it != contrastValues.end()) {
				double currentVal = *it;
				sum += std::abs(currentVal - prevVal);
				prevVal = currentVal;
				++it;
			}
		}

		return sum;
	}


	double Camera::stdev(double contrast) {
		static std::deque<double> contrastValues;
		double sum = 0;
		int expectedSize = 30;
		contrastValues.push_back(contrast);

		while (contrastValues.size() > expectedSize) {
			contrastValues.pop_front();
		}
		int countLastN = contrastValues.size();
		for (double val : contrastValues) {
			sum += val;
		}
		double mean = (countLastN > 0) ? (sum / countLastN) : 0.0;
		double variance = 0.0;
		for (double val : contrastValues) {
			variance += std::pow(val - mean, 2);
		}
		variance /= countLastN;
		double stdev = std::sqrt(variance);

		return stdev;
	}

	void RenderCombinedFrame(HWND shoo, const cv::Mat& llaaddoo) {
		// Calculate the stride
		RECT windowRect;
		GetClientRect(shoo, &windowRect);
		int windowWidth = windowRect.right - windowRect.left;
		int windowHeight = windowRect.bottom - windowRect.top;
		cv::Mat combinedFrame;
		cv::resize(llaaddoo, combinedFrame, cv::Size(windowWidth, windowHeight));

		int stride = combinedFrame.cols * combinedFrame.channels();

		// Get the size of the window
		

		// Calculate the aspect ratio of the combined frame
		double frameAspectRatio = static_cast<double>(combinedFrame.cols) / combinedFrame.rows;

		// Calculate the target size of the combined frame while preserving its aspect ratio
		int targetWidth, targetHeight;
		if (windowWidth / frameAspectRatio <= windowHeight) {
			// Fit by width
			targetWidth = windowWidth;
			targetHeight = static_cast<int>(windowWidth / frameAspectRatio);
		}
		else {
			// Fit by height
			targetHeight = windowHeight;
			targetWidth = static_cast<int>(windowHeight * frameAspectRatio);
		}

		// Initialize Direct2D
		ID2D1Factory* pD2DFactory = NULL;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

		if (SUCCEEDED(hr)) {
			// Create a render target
			ID2D1HwndRenderTarget* pRT = NULL;
			hr = pD2DFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(shoo, D2D1::SizeU(windowWidth, windowHeight)),
				&pRT
			);

			if (SUCCEEDED(hr)) {
				// Create bitmap properties
				D2D1_BITMAP_PROPERTIES bmpProperties;
				bmpProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
				bmpProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
				bmpProperties.dpiX = 96.0f;
				bmpProperties.dpiY = 96.0f;

				// Create a bitmap
				ID2D1Bitmap* pBitmap = NULL;
				hr = pRT->CreateBitmap(
					D2D1::SizeU(targetWidth, targetHeight),
					combinedFrame.data,
					stride,
					&bmpProperties,
					&pBitmap
				);

				if (SUCCEEDED(hr)) {
					// Draw the bitmap within the render target
					pRT->BeginDraw();
					pRT->DrawBitmap(pBitmap, D2D1::RectF(0, 0, windowWidth, windowHeight));
					pRT->EndDraw();

					// Release bitmap
					pBitmap->Release();
				}

				// Release render target
				pRT->Release();
			}

			// Release Direct2D factory
			pD2DFactory->Release();
		}
	}


};




#endif