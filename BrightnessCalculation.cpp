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
		double Sij = 0.0;

		if (!f1.empty() && !f20.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					uchar* r1 = f1.ptr<uchar>(y, x);
					uchar* r2 = f2.ptr<uchar>(y, x);
					uchar* r3 = f3.ptr<uchar>(y, x);
					uchar* r4 = f4.ptr<uchar>(y, x);
					uchar* r5 = f5.ptr<uchar>(y, x);
					uchar* r6 = f6.ptr<uchar>(y, x);
					uchar* r7 = f7.ptr<uchar>(y, x);
					uchar* r8 = f8.ptr<uchar>(y, x);
					uchar* r9 = f9.ptr<uchar>(y, x);
					uchar* r10 = f10.ptr<uchar>(y, x);
					uchar* r11 = f11.ptr<uchar>(y, x);
					uchar* r12 = f12.ptr<uchar>(y, x);
					uchar* r13 = f13.ptr<uchar>(y, x);
					uchar* r14 = f14.ptr<uchar>(y, x);
					uchar* r15 = f15.ptr<uchar>(y, x);
					uchar* r16 = f16.ptr<uchar>(y, x);
					uchar* r17 = f17.ptr<uchar>(y, x);
					uchar* r18 = f18.ptr<uchar>(y, x);
					uchar* r19 = f19.ptr<uchar>(y, x);
					double n1 = static_cast<int>(*r1);
					double n2 = static_cast<int>(*r2);
					double n3 = static_cast<int>(*r3);
					double n4 = static_cast<int>(*r4);
					double n5 = static_cast<int>(*r5);
					double n6 = static_cast<int>(*r6);
					double n7 = static_cast<int>(*r7);
					double n8 = static_cast<int>(*r8);
					double n9 = static_cast<int>(*r9);
					double n10 = static_cast<int>(*r10);
					double n11 = static_cast<int>(*r11);
					double n12 = static_cast<int>(*r12);
					double n13 = static_cast<int>(*r13);
					double n14 = static_cast<int>(*r14);
					double n15 = static_cast<int>(*r15);
					double n16 = static_cast<int>(*r16);
					double n17 = static_cast<int>(*r17);
					double n18 = static_cast<int>(*r18);
					double n19 = static_cast<int>(*r19);

					Sij = abs(n1 - n2) + 
						abs(n2 - n3) + 
						abs(n3 - n4) + 
						abs(n4 - n5) + 
						abs(n5 - n6) + 
						abs(n6 - n7) +
						abs(n7 - n8) +
						abs(n8 - n9) +
						abs(n9 - n10) +
						abs(n10 - n11) +
						abs(n11 - n12) +
						abs(n12 - n13) +
						abs(n13 - n14) +
						abs(n14 - n15) +
						abs(n15 - n16) +
						abs(n16 - n17) +
						abs(n17 - n18) +
						abs(n18 - n19);
					/*Pij = abs(f1.at<uchar>(y, x) - f2.at<uchar>(y, x)) +
						abs(f2.at<uchar>(y, x) - f3.at<uchar>(y, x)) +
						abs(f3.at<uchar>(y, x) - f4.at<uchar>(y, x)) +
						abs(f4.at<uchar>(y, x) - f5.at<uchar>(y, x)) +
						abs(f5.at<uchar>(y, x) - f6.at<uchar>(y, x)) +
						abs(f6.at<uchar>(y, x) - f7.at<uchar>(y, x)) +
						abs(f7.at<uchar>(y, x) - f8.at<uchar>(y, x)) +
						abs(f8.at<uchar>(y, x) - f9.at<uchar>(y, x)) +
						abs(f9.at<uchar>(y, x) - f10.at<uchar>(y, x)) +
						abs(f10.at<uchar>(y, x) - f11.at<uchar>(y, x)) +
						abs(f11.at<uchar>(y, x) - f12.at<uchar>(y, x)) +
						abs(f12.at<uchar>(y, x) - f13.at<uchar>(y, x)) +
						abs(f13.at<uchar>(y, x) - f14.at<uchar>(y, x)) +
						abs(f14.at<uchar>(y, x) - f15.at<uchar>(y, x)) +
						abs(f15.at<uchar>(y, x) - f16.at<uchar>(y, x)) +
						abs(f16.at<uchar>(y, x) - f17.at<uchar>(y, x)) +
						abs(f17.at<uchar>(y, x) - f18.at<uchar>(y, x)) +
						abs(f18.at<uchar>(y, x) - f19.at<uchar>(y, x)) +
						abs(f19.at<uchar>(y, x) - f20.at<uchar>(y, x));*/

				}
			}
		}
		else {
			return 0.0;
		}
		
		return Sij;
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
