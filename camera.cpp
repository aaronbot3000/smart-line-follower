#include "camera.hpp"

VideoCapture camera(0);

Mat cap, resized, processed, canny;

void init_camera() {
	cap			= Mat(CAP_ROWS, CAP_COLS, CV_8UC3);
	resized		= Mat(ROWS, COLS, CV_8UC3);
	processed	= Mat(ROWS, COLS, CV_8UC1);
	canny		= Mat(ROWS, COLS, CV_8UC1);
}

void grab_frame() {
	camera >> cap;
	//cap = imread("testline.jpg");
	resize(cap, resized, Size(COLS, ROWS));
}

void process_frame() {
	static const int fromTo[] = {2, 0};
	mixChannels(&resized, 1, &processed, 1, fromTo, 1);
	GaussianBlur(processed, processed, 
			Size(GAUSSIAN_SIZE, GAUSSIAN_SIZE), GAUSSIAN_SIGMA);
	Canny(processed, canny, 
			CANNY_HIGH, CANNY_LOW, CANNY_APETURE);
	//threshold(processed, canny, 128, 255, THRESH_BINARY);
}

Mat grab_processed_frame() {
	grab_frame();
	process_frame();
	return canny;
}

void destroy_camera() {
	cap.release();
	resized.release();
	processed.release();
	camera.release();
}
