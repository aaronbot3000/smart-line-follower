#include "camera.hpp"

VideoCapture camera(0);

Mat cap, resized, processed, canny;

void init_camera() {
	cap			= Mat(ROWS, COLS, CV_8UC3);
	processed	= Mat(ROWS, COLS, CV_8UC1);
	canny		= Mat(ROWS, COLS, CV_8UC1);

	camera.set(CV_CAP_PROP_FRAME_WIDTH, COLS);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, ROWS);
}

void grab_frame() {
	camera >> cap;
}

void process_frame() {
	static const int fromTo[] = {2, 0};
	mixChannels(&cap, 1, &processed, 1, fromTo, 1);
	GaussianBlur(processed, processed, 
			Size(GAUSSIAN_SIZE, GAUSSIAN_SIZE), GAUSSIAN_SIGMA);
	Canny(processed, canny, 
			CANNY_HIGH, CANNY_LOW, CANNY_APETURE);
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
