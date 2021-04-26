#include <opencv2/opencv.hpp>
#include <iostream>
void my_mouse_callback(
	int event, int x, int y, int flags, void* param
);
cv::Rect box;
bool drawing_box = false;
// A little subroutine to draw a box onto an image
//
void draw_box(cv::Mat& img, cv::Rect box) {
	cv::rectangle(
		img,
		box.tl(),
		box.br(),
		cv::Scalar(0x00, 0x00, 0xff) /* red */
	);
}
int main(int argc, char* argv[]) {
	cv::namedWindow("Example2_11", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Log_Polar", cv::WINDOW_AUTOSIZE);
	// ( Note: could capture from a camera by giving a camera id as an int.)
	//
	cv::VideoCapture capture(0); //default camera
	double fps = capture.get(cv::CAP_PROP_FPS);
	cv::Size size(
		(int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
		(int)capture.get(cv::CAP_PROP_FRAME_HEIGHT)
	);
	cv::VideoWriter writer;//open operations, path/codecs, fps, size
	writer.open("C:/Users/ASUS/Downloads/the.avi", CV_FOURCC('M', 'J', 'P', 'G'), fps, size);
	cv::Mat logpolar_frame, bgr_frame;//frame data structures

	int rotation = 0;
	int rotation2 = 0;

	//drawing function
	box = cv::Rect(-1, -1, 0, 0);

	cv::setMouseCallback(
		"Example2_11",
		my_mouse_callback,
		(void*)&bgr_frame
	);
	std::vector<cv::Rect> boxVect;
	for (;;) {//main loop
		capture >> bgr_frame;// stream ctrl
		if (bgr_frame.empty()) break; // end if done

		cv::putText(
			bgr_frame, // Image to be drawn on
			"This Text", // write this (often from cv::format)
			cv::Point(200, 200), // Upper-left corner of text box
			cv::FONT_HERSHEY_PLAIN, // Font (e.g., cv::FONT_HERSHEY_PLAIN)
			5, // size (a multiplier, not "points"!)
			cv::Scalar(255, 255, 0), // Color, RGB form
			8, // Thickness of line
			8, // Connectedness, 4 or 8
			false // true='origin at lower left'
		);

		//image.copyTo(bgr_frame);
		//boxBuffer
		if (drawing_box) {
			boxVect.push_back(box);
		}
		for (cv::Rect& c : boxVect) {
			draw_box(bgr_frame, c);
		}
		//cv::imshow("Box Example", temp);

		cv::imshow("Example2_11", bgr_frame); //output1 bkg

		//write a circle
		//cv::Mat whiteMatrix(200, 200, CV_8UC3, cv::Scalar(255, 255, 255));Declaring a white matrix
		cv::Point center(500, 500);//Declaring the center point
		int radius = 50; //Declaring the radius
		cv::Scalar line_Color(255, 255, 0);//Color of the circle
		int thickness = 2;//thickens of the line
		//namedWindow("whiteMatrix");//Declaring a window to show the circle
		circle(bgr_frame, cv::Point(500, 500), radius, line_Color, thickness);//Using circle()function to draw the line

		//write a line
		// 
		cv::line(
			bgr_frame, // Image to be drawn on
			cv::Point(500, ++rotation), // First endpoint of line
			cv::Point(++rotation, 300), // Second endpoint of line
			line_Color, // Color, BGR form
			8, // Connectedness, 4 or 8
			0 // Bits of radius to treat as fraction
		);
		if (rotation == 800) rotation = 0;
		//cv::flip(bgr_frame, bgr_frame, 1);//flip the output frame 180 h
		imshow("Example2_11", bgr_frame);//Showing the circle

		cv::logPolar(
			bgr_frame, // Input color frame
			logpolar_frame, // Output log-polar frame
			cv::Point2f( // Centerpoint for log-polar transformation
				bgr_frame.cols / 2, // x
				bgr_frame.rows / 2 // y
			),
			30, // Magnitude (scale parameter)
			cv::WARP_FILL_OUTLIERS // Fill outliers with 'zero'
		);

		cv::flip(logpolar_frame, logpolar_frame, 0);//flip the output
		cv::Point2f src_center(logpolar_frame.cols / 2.0F, logpolar_frame.rows / 2.0F);
		cv::Mat rot_matrix = getRotationMatrix2D(src_center, ++rotation2, 1.0); //flip rx90° -> 270sx
		cv::Mat rotated_img(cv::Size(logpolar_frame.size().height, logpolar_frame.size().width), logpolar_frame.type());
		if (rotation == 360)rotation = 0;

		warpAffine(logpolar_frame, rotated_img, rot_matrix, logpolar_frame.size());


		imshow("flipped", rotated_img);
		//cv::imshow("Log_Polar", logpolar_frame);

		writer << logpolar_frame;
		char c = cv::waitKey(10);
		if (c == 27) break; // allow the user to break out
	}
	capture.release();
}

// This is our mouse callback. If the user
// presses the left button, we start a box.
// When the user releases that button, then we
// add the box to the current image. When the
// mouse is dragged (with the button down) we
// resize the box.
//
void my_mouse_callback(
	int event, int x, int y, int flags, void* param
) {
	cv::Mat& image = *(cv::Mat*)param;
	switch (event) {
	case cv::EVENT_MOUSEMOVE: {
		if (drawing_box) {
			box.width = x - box.x;
			box.height = y - box.y;
		}
	}
							break;
	case cv::EVENT_LBUTTONDOWN: {
		drawing_box = true;
		box = cv::Rect(x, y, 0, 0);
	}
							  break;
	case cv::EVENT_LBUTTONUP: {
		drawing_box = false;
		if (box.width < 0) {
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0) {
			box.y += box.height;
			box.height *= -1;
		}
		draw_box(image, box);
	}
							break;
	}
}