#ifndef CAMERA_H
#define CAMERA_H
#include "opencv2/opencv.hpp"
#include "daq.h"
#include <Windows.h>
#include "BrightnessCalculation.h"
#include "preferencemanager.h"

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
	BrightnessClass my;
	PreferenceManager pref;
	Camera() : getId(id) {

	}
	void setStopCamera(bool stop);
	bool getstopCamera();
	void setDepositionBool(bool stop);
	bool getDepositionBool();
	void start();
	void drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness);
	double meanofBri(cv::Mat& iframe);
	std::deque<double>& GetPZTvolt();
	std::deque<double>& GetBrightData();
	void setBrightness(cv::Mat& tmpcalcFrame);
	void setPztVolt(double dddd);
	const double getBrightness();
	const double getUpdateofPzt();
	void DisplayCameraFrame(HWND hWnd, HWND hWn);
	bool iscapture = FALSE;
	void Camera::setCaptureScreenBool(bool stop);
	bool Camera::getCaptureScreenBool();


};




#endif