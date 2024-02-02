#ifndef BRIGHTNESS_CALCULATION_H
#define BRIGHTNESS_CALCULATION_H

#include "opencv2/opencv.hpp"
#include "MyUi.h"

class BrightnessClass {
public:
	cv::Mat frame, grayFrame;
	std::deque<double> framedata;
	double upperlimit, meanCV, variance;
	int width, height;
	MyUI myUIInstance;
	BrightnessClass();
	~BrightnessClass();
	void setUpperlimit(double uplimit);
	double getUpperlimit();
	double differencesOf(cv::Mat& iframe);
	double BrightnessClass::pdevidesum(cv::Mat& iframe);
};



#endif
