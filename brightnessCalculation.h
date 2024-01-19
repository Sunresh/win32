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

	double avg() {
		int height = grayFrame.rows;
		int width = grayFrame.cols;

		static cv::Mat f1, f2, f3;

		double th1 = 0.12 * 255;
		double th2 = 0.25 * 255;
		double th3 = 0.5 * 255;
		double th4 = 0.75 * 255;

		if (grayFrame.empty()) {
			// Handling if the input frame is empty
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
		else {
			// Randomize the assignment of frames for comparison
			int randNum = rand() % 3;
			if (randNum == 0) {
				f3 = f2.clone();
				f2 = f1.clone();
				f1 = grayFrame.clone();
			}
			else if (randNum == 1) {
				f1 = f3.clone();
				f3 = f2.clone();
				f2 = grayFrame.clone();
			}
			else {
				f2 = f1.clone();
				f1 = f3.clone();
				f3 = grayFrame.clone();
			}
		}

		double sumPij = 0.0;
		double previousPixelDiff = 0.0;

		if (!f1.empty() && !f3.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					// Calculate the brightness difference based on the thresholds
					int cf1 = f1.at<uchar>(y, x);
					int cf2 = f2.at<uchar>(y, x);
					int cf3 = f3.at<uchar>(y, x);
					//int pixelDiff = abs(3 * f1.at<uchar>(y, x) - f2.at<uchar>(y, x) - f3.at<uchar>(y, x));
					int pixelDiff = abs(3 * cf1 - cf2 - cf3);

					if (pixelDiff > th3 && pixelDiff <= th4) {
						double diffChange = previousPixelDiff - pixelDiff;
						// Apply previous pixelDiff comparison logic
						if (th1 < diffChange && diffChange < th2) {
							if (previousPixelDiff < pixelDiff) {
								sumPij += pixelDiff * 4; // Multiply by 8 if current is greater than previous
							}
							else {
								sumPij += pixelDiff * 0.0001; // Multiply by 0.001 if current is smaller than previous
							}
						}
						else if (th2 < diffChange && diffChange < th3) {
							if (previousPixelDiff < pixelDiff) {
								sumPij += pixelDiff * 3; // Multiply by 8 if current is greater than previous
							}
							else {
								sumPij += pixelDiff * 0.0001; // Multiply by 0.001 if current is smaller than previous
							}
						}
						else if (diffChange > th3) {
							if (previousPixelDiff < pixelDiff) {
								sumPij += pixelDiff * 2; // Multiply by 8 if current is greater than previous
							}
							else {
								sumPij += pixelDiff * 0.01; // Multiply by 0.001 if current is smaller than previous
							}
						}
						else {
							sumPij += pixelDiff * 8; // High weight for mid-high range differences
						}
					}
					else if (pixelDiff > th2 && pixelDiff <= th3) {
						sumPij += pixelDiff * 0.0001; // Low weight for mid-low range differences
					}
					else if (pixelDiff > th1 && pixelDiff <= th2) {
						sumPij += pixelDiff * 14; // Moderate weight for low-mid range differences
					}
					else {
						sumPij += pixelDiff * 7; // Normal contribution from differences within thresholds
					}
					previousPixelDiff = pixelDiff;
				}
			}
		}
		else {
			std::cerr << "Error: Frames f1 or f3 are empty." << std::endl;
			return 0.0;
		}

		if (sumPij > (255 * width * height) * 0.9) {
			static int frameCount = 0;
			if (frameCount % 2 == 0) {
				int randomValue = rand() % 3;
				sumPij = (255 * width * height) * 0.9 * randomValue / 2;
			}
			else if (frameCount % 2 == 1) {
				int randomValue = rand() % 3;
				sumPij = 0.1 * randomValue / 2;
			}
			frameCount++;
		}
		else if (sumPij < 0) {
			static int frameCount = 0;
			if (frameCount % 2 == 0) {
				int randomValue = rand() % 3;
				sumPij = 0.1 * randomValue / 2;
			}
			else if (frameCount % 2 == 1) {
				int randomValue = rand() % 3;
				sumPij = 1 * randomValue / 2;
			}
			frameCount++;
		}

		double contrast = sumPij / (255 * width * height);
		return contrast;
	}

	double avgot() {
		setUpperlimit(1);
		double sumPij = 0.0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				sumPij += grayFrame.at<uchar>(y, x);
			}
		}
		double contrast = sumPij / (255 * width * height);
		return contrast;
	}

	double differencesOf() {
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
		double Pij = 0.0;
		if (!f1.empty() && !f30.empty()) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					int cf1 = f1.at<uchar>(y, x);
					int cf2 = f2.at<uchar>(y, x);
					int cf3 = f3.at<uchar>(y, x);
					int cf4 = f4.at<uchar>(y, x);
					int cf5 = f5.at<uchar>(y, x);
					int cf6 = f6.at<uchar>(y, x);
					int cf7 = f7.at<uchar>(y, x);
					int cf8 = f8.at<uchar>(y, x);
					int cf9 = f9.at<uchar>(y, x);
					int cf10 = f10.at<uchar>(y, x);
					int cf11 = f11.at<uchar>(y, x);
					int cf12 = f12.at<uchar>(y, x);
					int cf13 = f13.at<uchar>(y, x);
					int cf14 = f14.at<uchar>(y, x);
					int cf15 = f15.at<uchar>(y, x);
					int cf16 = f16.at<uchar>(y, x);
					int cf17 = f17.at<uchar>(y, x);
					int cf18 = f18.at<uchar>(y, x);
					int cf19 = f19.at<uchar>(y, x);
					int cf20 = f20.at<uchar>(y, x);
					int cf21 = f21.at<uchar>(y, x);
					int cf22 = f22.at<uchar>(y, x);
					int cf23 = f23.at<uchar>(y, x);
					int cf24 = f24.at<uchar>(y, x);
					int cf25 = f25.at<uchar>(y, x);
					int cf26 = f26.at<uchar>(y, x);
					int cf27 = f27.at<uchar>(y, x);
					int cf28 = f28.at<uchar>(y, x);
					int cf29 = f29.at<uchar>(y, x);
					int cf30 = f30.at<uchar>(y, x);

					int pd30 = abs(cf1 - cf2);
					int pd31 = abs(cf2 - cf3);
					int pd32 = abs(cf3 - cf4);
					int pd33 = abs(cf4 - cf5);
					int pd34 = abs(cf5 - cf6);
					int pd35 = abs(cf6 - cf7);
					int pd36 = abs(cf7 - cf8);
					int pd37 = abs(cf8 - cf9);
					int pd38 = abs(cf9 - cf10);
					int pd39 = abs(cf10 - cf11);
					int pd40 = abs(cf11 - cf12);
					int pd41 = abs(cf12 - cf13);
					int pd42 = abs(cf13 - cf14);
					int pd43 = abs(cf14 - cf15);
					int pd44 = abs(cf15 - cf16);
					int pd45 = abs(cf16 - cf17);
					int pd46 = abs(cf17 - cf18);
					int pd47 = abs(cf18 - cf19);
					int pd48 = abs(cf19 - cf20);
					int pd49 = abs(cf20 - cf21);
					int pd50 = abs(cf21 - cf22);
					int pd51 = abs(cf22 - cf23);
					int pd52 = abs(cf23 - cf24);
					int pd53 = abs(cf24 - cf25);
					int pd54 = abs(cf25 - cf26);
					int pd55 = abs(cf26 - cf27);
					int pd56 = abs(cf27 - cf28);
					int pd57 = abs(cf28 - cf29);
					int pd58 = abs(cf29 - cf30);

					Pij = pd30 + pd31 + pd32 + pd33 + pd34 + pd35 + pd36 + pd37 + pd38 +
						pd39 + pd40 + pd41 + pd42 + pd43 + pd44 + pd45 + pd46 + pd47 + pd48;
					//+ pd49 + pd50 + pd51 + pd52 + pd53 + pd54 + pd55 + pd56 + pd57 + pd58;
				}
			}
		}
		else {
			return 0.0;
		}
		return Pij;
	}

};

