#include "BrightnessCalculation.h"

BrightnessClass::BrightnessClass() {};
BrightnessClass::~BrightnessClass() {};
void BrightnessClass::setUpperlimit(double uplimit) {
	upperlimit = uplimit;
}
double BrightnessClass::getUpperlimit() {
	return upperlimit;
}

double BrightnessClass::differencesOf(cv::Mat& iframe) {
	try {
		frame = iframe;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
		int height = grayFrame.rows;
		int width = grayFrame.cols;
		static cv::Mat f1, f2, f3, f4, f5;
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
		else {
			f5 = f4.clone();
			f4 = f3.clone();
			f3 = f2.clone();
			f2 = f1.clone();
			f1 = grayFrame.clone();
		}

		double Sij = 0.0;
		double sf1 = 0.0;
		double sf2 = 0.0;
		double sf3 = 0.0;
		double sf4 = 0.0;
		double sf5 = 0.0;
		double n1, n2, n3, n4, n5;
		if (!f1.empty() && !f5.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					uchar* r1 = f1.ptr<uchar>(y, x);
					uchar* r2 = f2.ptr<uchar>(y, x);
					uchar* r3 = f3.ptr<uchar>(y, x);
					uchar* r4 = f4.ptr<uchar>(y, x);
					uchar* r5 = f5.ptr<uchar>(y, x);
					
					n1 = static_cast<int>(*r1);
					n2 = static_cast<int>(*r2);
					n3 = static_cast<int>(*r3);
					n4 = static_cast<int>(*r4);
					n5 = static_cast<int>(*r5);
					sf1 += n1;
					sf2 += n2;
					sf3 += n3;
					sf4 += n4;
					sf5 += n5;
				}
			}
		}
		else {
			return 0.0;
		}
		double result =10000* abs((n1 / sf1)-(n2 / sf2)); setUpperlimit(2);
		//double result = abs(n1-n2); setUpperlimit(500);
		//double result = sf1 / (255*height*width); setUpperlimit(10);
		return result;
	}
	catch (const std::exception& e) {
		myUIInstance.messi(e.what());
	}
	catch (...) {
		myUIInstance.mess(L"Unknown error occurred in BrightnessCalculation.");
	}
}

