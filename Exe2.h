#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
int main(int argc, char** argv) {
	cv::namedWindow("Example3", cv::WINDOW_AUTOSIZE);
	cv::VideoCapture cap;
	cap.open("C:/Users/ASUS/Downloads/sample.avi");
	cv::Mat frame;
	for (;;) {
		cap >> frame;
		if (frame.empty()) break; // Ran out of film
		cv::imshow("Example3", frame);
		if (cv::waitKey(1) >= 0) break;
	}
	return 0;
}