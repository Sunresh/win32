#ifndef CAMERA_H
#define CAMERA_H
#include "opencv2/opencv.hpp"
#include <NIDAQmx.h>
#include <Windows.h>
#include "BrightnessCalculation.h"
#include "preferencemanager.h"
#include "MyUi.h"
#include "ExportCsv.h"

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
	std::deque<double> brightData, pztVolt;
	double brightness = 0;
	bool startDepo = false;
	TaskHandle epvtask = nullptr;
	TaskHandle pztvtask = nullptr;
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
	ExportCSV csv;
	BrightnessClass my;
	MyUI myUIInstance;
	PreferenceManager pref;
	void setEV(double ephv = 2.0);
	double getEV();
	void setStopCamera(bool stop);
	bool getstopCamera();
	void setSQW(double stop);
	void Camera::pauseCamera();
	int cameraid = 0;
	std::string filepath;
	void setDepositionBool(bool stop);
	bool getDepositionBool();
	void start();
	void drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness);
	double meanofBri(cv::Mat& iframe);

	std::deque<double>& GetPZTvolt();
	std::deque<double>& GetBrightData();
	cv::Rect box; // Rectangle to be resized
	bool drawing_box = false;
	void setBrightness(cv::Mat& tmpcalcFrame);
	const double getBrightness();
	void DisplayCameraFrame();
	bool iscapture = FALSE;
	void Camera::setCaptureScreenBool(bool stop);
	bool Camera::getCaptureScreenBool();
	std::string current_filename ="ExampleND";
	void Camera::allgraph(cv::Mat& frame, std::deque<double>& graphValues, double upperLimit, const std::string& yxix);
	void Camera::cameraIndex();
	static void Camera::mouse_callback(int event, int x, int y, int flags, void* param);
	int Camera::getCameraId() const;
	std::string Camera::getFilePath() const;

};




#endif