#include "framework.h"
#include "WindowsProject1.h"
#include "opencv2/opencv.hpp"

class Camera {
private:
	int id = 0;
	int getId;
public:
	Camera(): getId(id) {

	}
	void start() {
		OutputDebugStringW(L"\naayyo\n");
	}
	void drawRectangle(cv::Mat& frame, int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness=1) {
		cv::rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), color, thickness);
	}
	double meanofBri(cv::Mat& iframe) {
		cv::Mat grayFrame;
		cv::cvtColor(iframe, grayFrame, cv::COLOR_BGR2GRAY);
		double width = grayFrame.cols;
		double height = grayFrame.rows;
		double variance = 0.0;
		double meanCV = cv::mean(grayFrame)[0];
		return meanCV;
	}


};