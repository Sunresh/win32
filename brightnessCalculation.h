#include "WindowsProject1.h"

class BrightnessClass {
public:
	cv::Mat frame, grayFrame;
	std::deque<double> framedata;
	double upperlimit, meanCV, variance;
	int width, height;

	BrightnessClass(cv::Mat& iframe) {
		frame = iframe;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
		width = grayFrame.cols;
		height = grayFrame.rows;
		variance = 0.0;
		meanCV = cv::mean(grayFrame)[0];
	};
	~BrightnessClass() {};
	void setUpperlimit(double uplimit) {
		upperlimit = uplimit;
	}
	double BrightnessClass::getUpperlimit() {
		return upperlimit;
	}

	double differencesOf() {
		int height = grayFrame.rows;
		int width = grayFrame.cols;
		static cv::Mat f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
			f11, f12, f13, f14, f15, f16, f17, f18, f19, f20;
		if (grayFrame.empty()) {
			std::cerr << "Error: Empty input frame." << std::endl;
			return 0.0;
		}

		if (f1.empty()) {
			f1 = grayFrame.clone();
		}
		else if (f2.empty()) {
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f3.empty()) {
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f4.empty()) {
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f5.empty()) {
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f6.empty()) {
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f7.empty()) {
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f8.empty()) {
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f9.empty()) {
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f10.empty()) {
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f11.empty()) {
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f12.empty()) {
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f13.empty()) {
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f14.empty()) {
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f15.empty()) {
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f16.empty()) {
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f17.empty()) {
			f17 = f16.clone();
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f18.empty()) {
			f18 = f17.clone();
			f17 = f16.clone();
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f19.empty()) {
			f19 = f18.clone();
			f18 = f17.clone();
			f17 = f16.clone();
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else if (f20.empty()) {
			f20 = f19.clone();
			f19 = f18.clone();
			f18 = f17.clone();
			f17 = f16.clone();
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		else {
			f20 = f19.clone();
			f19 = f18.clone();
			f18 = f17.clone();
			f17 = f16.clone();
			f16 = f15.clone();
			f15 = f14.clone();
			f14 = f13.clone();
			f13 = f12.clone();
			f12 = f11.clone();
			f11 = f10.clone();
			f10 = f9.clone();
			f9 = f8.clone();
			f8 = f7.clone();
			f7 = f6.clone();
			f6 = f5.clone();
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}
		double Pij = 0.0;
		if (!f1.empty() && !f20.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					Pij = abs(f1.at<uchar>(y, x) - f2.at<uchar>(y, x))+
					abs(f2.at<uchar>(y, x) - f3.at<uchar>(y, x))+
					abs(f3.at<uchar>(y, x) - f4.at<uchar>(y, x))+
					abs(f4.at<uchar>(y, x) - f5.at<uchar>(y, x))+
					abs(f5.at<uchar>(y, x) - f6.at<uchar>(y, x))+
					abs(f6.at<uchar>(y, x) - f7.at<uchar>(y, x))+
					abs(f7.at<uchar>(y, x) - f8.at<uchar>(y, x))+
					abs(f8.at<uchar>(y, x) - f9.at<uchar>(y, x))+
					abs(f9.at<uchar>(y, x) - f10.at<uchar>(y, x))+
					abs(f10.at<uchar>(y, x) - f11.at<uchar>(y, x))+
					abs(f11.at<uchar>(y, x) - f12.at<uchar>(y, x))+
					abs(f12.at<uchar>(y, x) - f13.at<uchar>(y, x))+
					abs(f13.at<uchar>(y, x) - f14.at<uchar>(y, x))+
					abs(f14.at<uchar>(y, x) - f15.at<uchar>(y, x))+
					abs(f15.at<uchar>(y, x) - f16.at<uchar>(y, x))+
					abs(f16.at<uchar>(y, x) - f17.at<uchar>(y, x))+
					abs(f17.at<uchar>(y, x) - f18.at<uchar>(y, x))+
					abs(f18.at<uchar>(y, x) - f19.at<uchar>(y, x))+
					abs(f19.at<uchar>(y, x) - f20.at<uchar>(y, x));

				}
			}
		}
		else {
			return 0.0;
		}
		return Pij;
	}




};

