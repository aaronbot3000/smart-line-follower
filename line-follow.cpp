#include "line-follow.hpp"

extern Mat cap, resized, processed, canny;
int main() {
	Mat frame;
#ifdef __x86_64
	Mat lines;
	namedWindow("Raw", CV_WINDOW_KEEPRATIO);
	namedWindow("Resized", CV_WINDOW_KEEPRATIO);
	namedWindow("Processed", CV_WINDOW_KEEPRATIO);
	namedWindow("canny", CV_WINDOW_KEEPRATIO);
	namedWindow("lines", CV_WINDOW_KEEPRATIO);
	init_outimg();
#endif

	init_camera();

	int i = 0;

	while (1) {
		frame = grab_processed_frame();
		get_lines(frame);
#ifdef __x86_64
		lines = get_line_image();
		imshow("Raw", cap);
		imshow("Resized", resized);
		imshow("Processed", processed);
		imshow("canny", canny);
		imshow("lines", lines);

		if (waitKey(20) == 'e') {
			break;
		}
#endif
		if (i == 30) {
			printf("run 30 frame\n");
			i = 0;
		}

		i++;
	}

	return 0;
}
