#include "camera.hpp"

#ifdef __x86_64
VideoCapture camera(1);
#elif
VideoCapture camera(0);
#endif

Mat cap, resized, processed, canny;

void init_camera() {
	cap			= Mat(ROWS, COLS, CV_8UC3);
	processed	= Mat(ROWS, COLS, CV_8UC1);
	canny		= Mat(ROWS, COLS, CV_8UC1);

	camera.set(CV_CAP_PROP_FRAME_WIDTH, COLS);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, ROWS);
	//camera.set(CV_CAP_PROP_CONTRAST, 1);
}

void grab_frame() {
	camera >> cap;
	//cap = imread("testline2.png");
}

void process_frame() {
	static const int fromTo[] = {1, 0};
	mixChannels(&cap, 1, &processed, 1, fromTo, 1);
	GaussianBlur(processed, processed, 
			Size(GAUSSIAN_SIZE, GAUSSIAN_SIZE), GAUSSIAN_SIGMA);
	threshold(processed, processed, THRESHOLD_LVL, 255, CV_THRESH_BINARY);
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
