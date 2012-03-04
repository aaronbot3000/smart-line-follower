#include "line-follow.hpp"

#ifdef __x86_64
extern Mat cap, resized, processed, canny;
#endif

int main() {
	Mat frame;
	char pic_com[3];
	char command;

	open_serial();

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
		vector<Scalar_<float> > lines = get_lines(frame);
		command = read_serial();
		if (command) {
			cout << "received stuff: " << command << endl;
			if (command == GET_LINE_DATA) { // Get line data
				if (lines.size() == 1) { // Tracking one line
					float xpos = lines[0][CENTER_X] / lines[0][MAGNITUDE];
					float angle = lines[0][THETA] / lines[0][MAGNITUDE];
					//float ypos = lines[0][2] / lines[0][3];
					int error = ((xpos - (COLS / 2)) / (COLS / 2)) * 255;

					if (error < 0)
						pic_com[0] = LEFT;
					else if (error > 0)
						pic_com[0] = RIGHT;
					else
						pic_com[0] = CONTINUE;
					pic_com[1] = (unsigned char)abs(error);
					if (angle < 0)
						pic_com[2] = (unsigned char)(-angle * 128 / PI);
					else
						pic_com[2] = (unsigned char)(255 - angle * 128 / PI);
					printf("error = %d\n", (int)((unsigned char)pic_com[1]));
					printf("angle = %u\n", pic_com[2]);
					write_serial(pic_com);
				}
				if (lines.size() >= 2) { // Two lines
					
				}
			}
		}

#ifdef __x86_64
		get_line_image(outdisplay);
		draw_found_lines(outdisplay);
		imshow("Raw", cap);
		imshow("Processed", processed);
		imshow("canny", canny);
		imshow("lines", outdisplay);

		if (waitKey(50) == 'q') {
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
