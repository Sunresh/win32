#ifndef BRIGHTNESS_CALCULATION_H
#define BRIGHTNESS_CALCULATION_H

#include "opencv2/opencv.hpp"
#include "MyUi.h"

class BrightnessClass {
public:
	cv::Mat frame, grayFrame;
	std::deque<double> framedata;
	int width, height;
	MyUI myUIInstance;
	BrightnessClass() {};
	~BrightnessClass() {};
	void BrightnessClass::differencesOf(cv::Mat& iframe, double& averageBrightness, double& diff, double& norm, double& sizze) {
		try {
			cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
			int height = grayFrame.rows;
			int width = grayFrame.cols;
			
			double sumPij = 0.0;
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					sumPij += static_cast<double>(grayFrame.at<uchar>(y, x));
				}
			}
			averageBrightness = sumPij / (255 * width * height);
		}
		catch (const std::exception& e) {
			myUIInstance.messi(e.what());
		}
		catch (...) {
			myUIInstance.mess(L"Unknown error occurred in BrightnessCalculation.");
		}

		/*try {
			frame = iframe;
			cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
			int height = grayFrame.rows;
			int width = grayFrame.cols;
			static cv::Mat f1;
			if (grayFrame.empty()) {
				std::cerr << "Error: Empty input frame." << std::endl;
				return ;
			}

			if (f1.empty()) {
				f1 = grayFrame.clone();
			}
			else {
				f1 = grayFrame.clone();
			}
			double sf1 = 0.0;
			double normalize = 0;
			double n1=0;
			if (!f1.empty()) {
				for (int y = 0; y < height; ++y) {
					for (int x = 0; x < width; ++x) {
						uchar* r1 = f1.ptr<uchar>(y, x);
						n1 = static_cast<int>(*r1);
						sf1 += n1;
					}
				}
				for (int y = 0; y < height; ++y) {
					for (int x = 0; x < width; ++x) {
						uchar* r1 = f1.ptr<uchar>(y, x);
						int nana = static_cast<int>(*r1);
						normalize = nana / sf1;
					}
				}
			}
			else {
				return ;
			}
			diff = n1;
			norm = 10000 * normalize;
			averageBrightness = sf1 / (255*height*width);
			sizze = height * width;
		}
		catch (const std::exception& e) {
			myUIInstance.messi(e.what());
		}
		catch (...) {
			myUIInstance.mess(L"Unknown error occurred in BrightnessCalculation.");
		}*/
	}

};



#endif
