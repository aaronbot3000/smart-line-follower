#include "line-follow.hpp"

extern Mat cap, resized, processed, canny;
int main() {
	Mat frame, lines;
	namedWindow("Raw", CV_WINDOW_KEEPRATIO);
	namedWindow("Resized", CV_WINDOW_KEEPRATIO);
	namedWindow("Processed", CV_WINDOW_KEEPRATIO);
	namedWindow("canny", CV_WINDOW_KEEPRATIO);
	namedWindow("lines", CV_WINDOW_KEEPRATIO);
	init_camera();
	init_outimg();

	int i = 0;

	while (1) {
		if (waitKey(20) == 'e') {
			break;
		}
		frame = grab_processed_frame();
		get_lines(frame);
		lines = get_line_image();
		imshow("Raw", cap);
		imshow("Resized", resized);
		imshow("Processed", processed);
		imshow("canny", canny);
		imshow("lines", lines);
		if (i == 30) {
			printf("run 30 frame\n");
			i = 0;
		}

		i++;
	}

	return 0;
}
