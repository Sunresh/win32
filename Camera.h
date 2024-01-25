#ifndef CAMERA_H
#define CAMERA_H
#include "opencv2/opencv.hpp"
#include "daq.h"
#include <Windows.h>
#include "BrightnessCalculation.h"
#include "preferencemanager.h"
#include "MyUi.h"

class Camera {
public:
	int id = 0;
	cv::VideoCapture cap;
	int getId;
	bool stopCamera = true;
	std::deque<double> brightData, pztVolt;
	double brightness = 0;
	bool startDepo = false;
	MyDaq daq;
	double sqw = 15;
	double sqh = 15;
	double sqx1 = 15;
	double sqy1 = 15;
	double time = 15;
	double uth = 15;
	double lth = 15;
	double pztmax = 15;
	double epv = 2;
	double kk = 15;

	BrightnessClass my;
	MyUI myUIInstance;
	PreferenceManager pref;
	void setEV(double ephv = 2.0);
	double getEV();
	void setStopCamera(bool stop);
	bool getstopCamera();
	void setSQW(double stop);
	void Camera::pauseCamera();

	void setDepositionBool(bool stop);
	bool getDepositionBool();
	void start();
	void drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness);
	double meanofBri(cv::Mat& iframe);

	std::deque<double>& GetPZTvolt();
	std::deque<double>& GetBrightData();

	void setBrightness(cv::Mat& tmpcalcFrame);
	const double getBrightness();
	void DisplayCameraFrame(HWND hWnd, HWND hWn);
	bool iscapture = FALSE;
	void Camera::setCaptureScreenBool(bool stop);
	bool Camera::getCaptureScreenBool();


};




#endif