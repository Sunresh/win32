#ifndef BRIGHTNESS_CALCULATION_H
#define BRIGHTNESS_CALCULATION_H

#include "opencv2/opencv.hpp"


class BrightnessClass {
public:
	cv::Mat frame, grayFrame;
	std::deque<double> framedata;
	double upperlimit, meanCV, variance;
	int width, height;

	BrightnessClass();
	~BrightnessClass();
	void setUpperlimit(double uplimit);
	double getUpperlimit();
	double differencesOf(cv::Mat& iframe);

};



#endif

