#include "line-follow.hpp"

#ifdef __x86_64
extern Mat cap, resized, processed, canny;
#endif

int main() {
	Mat frame;

#ifdef __x86_64
	Mat outdisplay(ROWS, COLS, CV_8UC3);
	namedWindow("Raw", CV_WINDOW_KEEPRATIO);
	namedWindow("Processed", CV_WINDOW_KEEPRATIO);
	namedWindow("canny", CV_WINDOW_KEEPRATIO);
	namedWindow("lines", CV_WINDOW_KEEPRATIO);
#endif

	init_camera();

	int i = 0;

	while (1) {
		frame = grab_processed_frame();
		get_lines(frame);

#ifdef __x86_64
		get_line_image(outdisplay);
		draw_found_lines(outdisplay);
		imshow("Raw", cap);
		imshow("Processed", processed);
		imshow("canny", canny);
		imshow("lines", outdisplay);

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
