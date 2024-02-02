#include "BrightnessCalculation.h"

BrightnessClass::BrightnessClass() {};
BrightnessClass::~BrightnessClass() {};
void BrightnessClass::setUpperlimit(double uplimit) {
	upperlimit = uplimit;
}
double BrightnessClass::getUpperlimit() {
	return upperlimit;
}


double BrightnessClass::pdevidesum(cv::Mat& iframe) {
	try {
		frame = iframe;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);

		int height = grayFrame.rows;
		int width = grayFrame.cols;
		static cv::Mat f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
			f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
			f21, f22, f23, f24, f25, f26, f27, f28, f29, f30;

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
		else if (f21.empty()) {
			f21 = f20.clone();
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
		else if (f22.empty()) {
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f23.empty()) {
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f24.empty()) {
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f25.empty()) {
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f26.empty()) {
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f27.empty()) {
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f28.empty()) {
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f29.empty()) {
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f30.empty()) {
			f30 = f29.clone();
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
			f30 = f29.clone();
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		double sf1 = 0.0;
		double sf2 = 0.0;
		double sf3 = 0.0;
		double sf4 = 0.0;
		double sf5 = 0.0;
		double sf6 = 0.0;
		double sf7 = 0.0;
		double sf8 = 0.0;
		double sf9 = 0.0;
		double sf10 = 0.0;
		double sf11 = 0.0;
		double sf12 = 0.0;
		double sf13 = 0.0;
		double sf14 = 0.0;
		double sf15 = 0.0;
		double sf16 = 0.0;
		double sf17 = 0.0;
		double sf18 = 0.0;
		double sf19 = 0.0;
		double sf20 = 0.0;
		double sf21 = 0.0;
		double sf22 = 0.0;
		double n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20, n21, n22, n23, n24, n25, n26, n27, n28, n29;
		if (!f1.empty() && !f30.empty()) {
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
					uchar* r20 = f20.ptr<uchar>(y, x);
					uchar* r21 = f21.ptr<uchar>(y, x);
					uchar* r22 = f22.ptr<uchar>(y, x);
					uchar* r23 = f23.ptr<uchar>(y, x);
					uchar* r24 = f24.ptr<uchar>(y, x);
					uchar* r25 = f25.ptr<uchar>(y, x);
					uchar* r26 = f26.ptr<uchar>(y, x);
					uchar* r27 = f27.ptr<uchar>(y, x);
					uchar* r28 = f28.ptr<uchar>(y, x);
					uchar* r29 = f29.ptr<uchar>(y, x);

					n1 = static_cast<int>(*r1);
					n2 = static_cast<int>(*r2);
					n3 = static_cast<int>(*r3);
					n4 = static_cast<int>(*r4);
					n5 = static_cast<int>(*r5);
					n6 = static_cast<int>(*r6);
					n7 = static_cast<int>(*r7);
					n8 = static_cast<int>(*r8);
					n9 = static_cast<int>(*r9);
					n10 = static_cast<int>(*r10);
					n11 = static_cast<int>(*r11);
					n12 = static_cast<int>(*r12);
					n13 = static_cast<int>(*r13);
					n14 = static_cast<int>(*r14);
					n15 = static_cast<int>(*r15);
					n16 = static_cast<int>(*r16);
					n17 = static_cast<int>(*r17);
					n18 = static_cast<int>(*r18);
					n19 = static_cast<int>(*r19);
					n20 = static_cast<int>(*r20);
					n21 = static_cast<int>(*r21);
					n22 = static_cast<int>(*r22);
					n23 = static_cast<int>(*r23);
					n24 = static_cast<int>(*r24);
					n25 = static_cast<int>(*r25);
					n26 = static_cast<int>(*r26);
					n27 = static_cast<int>(*r27);
					n28 = static_cast<int>(*r28);
					n29 = static_cast<int>(*r29);
					sf1 += n1;
					sf2 += n2;
					sf3 += n3;
					sf4 += n4;
					sf5 += n5;
					sf6 += n6;
					sf7 += n7;
					sf8 += n8;
					sf9 += n9;
					sf10 += n10;
					sf11 += n11;
					sf12 += n12;
					sf13 += n13;
					sf14 += n14;
					sf15 += n15;
					sf16 += n16;
					sf17 += n17;
					sf18 += n18;
					sf19 += n19;
					sf20 += n20;
					sf21 += n21;
					sf22 += n22;

				}
			}
		}
		else {
			return 0.0;
		}
		Sij = abs((n1 / sf1) - (n2 / sf2)) +
			abs((n2 / sf2) - (n3 / sf3)) +
			abs((n3 / sf3) - (n4 / sf4)) +
			abs((n4 / sf4) - (n5 / sf5)) +
			abs((n5 / sf5) - (n6 / sf6)) +
			abs((n6 / sf6) - (n7 / sf7)) +
			abs((n7 / sf7) - (n8 / sf8)) +
			abs((n8 / sf8) - (n9 / sf9)) +
			abs((n9 / sf9) - (n10 / sf10)) +
			abs((n10 / sf10) - (n11 / sf11)) +
			abs((n11 / sf11) - (n12 / sf12)) +
			abs((n12 / sf12) - (n13 / sf13)) +
			abs((n13 / sf13) - (n14 / sf14)) +
			abs((n14 / sf14) - (n15 / sf15)) +
			abs((n15 / sf15) - (n16 / sf16)) +
			abs((n16 / sf16) - (n17 / sf17)) +
			abs((n17 / sf17) - (n18 / sf18)) +
			abs((n18 / sf18) - (n19 / sf19)) +
			abs((n19 / sf19) - (n20 / sf20)) +
			abs((n20 / sf20) - (n21 / sf21)) +
			abs((n21 / sf21) - (n22 / sf22)) +
			abs((n22 / sf22) - (n23)) +
			abs(n23 - n24) +
			abs(n24 - n25) +
			abs(n25 - n26) +
			abs(n26 - n27) +
			abs(n27 - n28) +
			abs(n28 - n29);

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

double BrightnessClass::differencesOf(cv::Mat& iframe) {
	try {
		frame = iframe;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);

		int height = grayFrame.rows;
		int width = grayFrame.cols;
		static cv::Mat f1, f2, f3, f4, f5, f6, f7, f8, f9, f10,
			f11, f12, f13, f14, f15, f16, f17, f18, f19, f20,
			f21, f22, f23, f24, f25, f26, f27, f28, f29, f30;

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
		else if (f21.empty()) {
			f21 = f20.clone();
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
		else if (f22.empty()) {
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f23.empty()) {
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f24.empty()) {
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f25.empty()) {
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f26.empty()) {
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f27.empty()) {
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f28.empty()) {
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f29.empty()) {
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		else if (f30.empty()) {
			f30 = f29.clone();
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
			f30 = f29.clone();
			f29 = f28.clone();
			f28 = f27.clone();
			f27 = f26.clone();
			f26 = f25.clone();
			f25 = f24.clone();
			f24 = f23.clone();
			f23 = f22.clone();
			f22 = f21.clone();
			f21 = f20.clone();
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
		double sf1 = 0.0;
		double sf2 = 0.0;
		double sf3 = 0.0;
		double sf4 = 0.0;
		double sf5 = 0.0;
		double sf6 = 0.0;
		double sf7 = 0.0;
		double sf8 = 0.0;
		double sf9 = 0.0;
		double sf10 = 0.0;
		double sf11 = 0.0;
		double sf12 = 0.0;
		double sf13 = 0.0;
		double sf14 = 0.0;
		double sf15 = 0.0;
		double sf16 = 0.0;
		double sf17 = 0.0;
		double sf18 = 0.0;
		double sf19 = 0.0;
		double sf20 = 0.0;
		double sf21 = 0.0;
		double sf22 = 0.0;
		double n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, n20, n21, n22, n23, n24, n25, n26, n27, n28, n29;
		if (!f1.empty() && !f30.empty()) {
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
					uchar* r20 = f20.ptr<uchar>(y, x);
					uchar* r21 = f21.ptr<uchar>(y, x);
					uchar* r22 = f22.ptr<uchar>(y, x);
					uchar* r23 = f23.ptr<uchar>(y, x);
					uchar* r24 = f24.ptr<uchar>(y, x);
					uchar* r25 = f25.ptr<uchar>(y, x);
					uchar* r26 = f26.ptr<uchar>(y, x);
					uchar* r27 = f27.ptr<uchar>(y, x);
					uchar* r28 = f28.ptr<uchar>(y, x);
					uchar* r29 = f29.ptr<uchar>(y, x);

					n1 = static_cast<int>(*r1);
					n2 = static_cast<int>(*r2);
					n3 = static_cast<int>(*r3);
					n4 = static_cast<int>(*r4);
					n5 = static_cast<int>(*r5);
					n6 = static_cast<int>(*r6);
					n7 = static_cast<int>(*r7);
					n8 = static_cast<int>(*r8);
					n9 = static_cast<int>(*r9);
					n10 = static_cast<int>(*r10);
					n11 = static_cast<int>(*r11);
					n12 = static_cast<int>(*r12);
					n13 = static_cast<int>(*r13);
					n14 = static_cast<int>(*r14);
					n15 = static_cast<int>(*r15);
					n16 = static_cast<int>(*r16);
					n17 = static_cast<int>(*r17);
					n18 = static_cast<int>(*r18);
					n19 = static_cast<int>(*r19);
					n20 = static_cast<int>(*r20);
					n21 = static_cast<int>(*r21);
					n22 = static_cast<int>(*r22);
					n23 = static_cast<int>(*r23);
					n24 = static_cast<int>(*r24);
					n25 = static_cast<int>(*r25);
					n26 = static_cast<int>(*r26);
					n27 = static_cast<int>(*r27);
					n28 = static_cast<int>(*r28);
					n29 = static_cast<int>(*r29);
					sf1 += n1;
					sf2 += n2;
					sf3 += n3;
					sf4 += n4;
					sf5 += n5;
					sf6 += n6;
					sf7 += n7;
					sf8 += n8;
					sf9 += n9;
					sf10 += n10;
					sf11 += n11;
					sf12 += n12;
					sf13 += n13;
					sf14 += n14;
					sf15 += n15;
					sf16 += n16;
					sf17 += n17;
					sf18 += n18;
					sf19 += n19;
					sf20 += n20;
					sf21 += n21;
					sf22 += n22;

				}
			}
		}
		else {
			return 0.0;
		}
		Sij = abs((n1 / sf1) - (n2 / sf2)) +
			abs((n2 / sf2) - (n3 / sf3)) +
			abs((n3 / sf3) - (n4 / sf4)) +
			abs((n4 / sf4) - (n5 / sf5)) +
			abs((n5 / sf5) - (n6 / sf6)) +
			abs((n6 / sf6) - (n7 / sf7)) +
			abs((n7 / sf7) - (n8 / sf8)) +
			abs((n8 / sf8) - (n9 / sf9)) +
			abs((n9 / sf9) - (n10 / sf10)) +
			abs((n10 / sf10) - (n11 / sf11)) +
			abs((n11 / sf11) - (n12 / sf12)) +
			abs((n12 / sf12) - (n13 / sf13)) +
			abs((n13 / sf13) - (n14 / sf14)) +
			abs((n14 / sf14) - (n15 / sf15)) +
			abs((n15 / sf15) - (n16 / sf16)) +
			abs((n16 / sf16) - (n17 / sf17)) +
			abs((n17 / sf17) - (n18 / sf18)) +
			abs((n18 / sf18) - (n19 / sf19)) +
			abs((n19 / sf19) - (n20 / sf20)) +
			abs((n20 / sf20) - (n21 / sf21)) +
			abs((n21 / sf21) - (n22 / sf22)) +
			abs((n22 / sf22) - (n23)) +
			abs(n23 - n24) +
			abs(n24 - n25) +
			abs(n25 - n26) +
			abs(n26 - n27) +
			abs(n27 - n28) +
			abs(n28 - n29);

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


