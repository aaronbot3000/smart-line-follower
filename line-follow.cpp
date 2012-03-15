#include "line-follow.hpp"

#ifdef __x86_64
extern Mat cap, resized, processed, canny;
#endif

static char pic_com[3];
int main() {
	Mat frame;
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
				if (!track_two_lines(lines)) {
					if (!track_one_line(lines)) {
						pic_com[0] = NO_LINE;
						pic_com[1] = 0;
						pic_com[2] = 0;
					}
				}
			}
			if (command == NUM_LINES_FOUND) {
				pic_com[0] = lines.size();
			}

			if (pic_com[0] == LEFT)
				printf("LEFT\n");
			if (pic_com[0] == RIGHT)
				printf("RIGHT\n");
			if (pic_com[0] == CONTINUE)
				printf("CONTINUE\n");
			if (pic_com[0] == FULL_LEFT_TURN)
				printf("FULL_LEFT_TURN\n");
			if (pic_com[0] == FULL_RIGHT_TURN)
				printf("FULL_RIGHT_TURN\n");
			if (pic_com[0] == FOUND_T)
				printf("FOUND_T\n");
			write_serial(pic_com);
		}

#ifdef __x86_64
		if (!track_two_lines(lines))
			if (!track_one_line(lines))
				pic_com[0] = 0;

		if (pic_com[0] == LEFT)
			printf("LEFT\n");
		if (pic_com[0] == RIGHT)
			printf("RIGHT\n");
		if (pic_com[0] == CONTINUE)
			printf("CONTINUE\n");
		if (pic_com[0] == FULL_LEFT_TURN)
			printf("FULL_LEFT_TURN\n");
		if (pic_com[0] == FULL_RIGHT_TURN)
			printf("FULL_RIGHT_TURN\n");
		if (pic_com[0] == FOUND_T)
			printf("FOUND_T\n");
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

bool track_one_line(Vector<Scalar_<float> > lines) {
	if (lines.size() < 1) {
		return false;
	}
	// Tracking one line
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

	// Theta = PI is vertical
	// Positive is left
	// Negative is right
	pic_com[1] = (unsigned char)abs(error);
	if (angle < 0) {
		pic_com[2] = (unsigned char)(-angle * 128 / (PI / 2));
	}
	else
		pic_com[2] = (unsigned char)(255 - angle * 128 / (PI / 2));

	printf("error = %d\n", (int)((unsigned char)pic_com[1]));
	printf("angle = %u\n", (unsigned char)pic_com[2]);
	return true;
}

bool track_two_lines(Vector<Scalar_<float> > lines) {
	if (lines.size() < 2) {
		return false;
	}
	float xpos1 = lines[0][CENTER_X] / lines[0][MAGNITUDE];
	float xpos2 = lines[1][CENTER_X] / lines[1][MAGNITUDE];

	float ypos1 = lines[0][CENTER_Y] / lines[0][MAGNITUDE];
	float ypos2 = lines[1][CENTER_Y] / lines[1][MAGNITUDE];
	if (ypos1 < ypos2) {
		printf("ypos1 = %f ypos2 = %f\n", ypos1, ypos2);
		return false;
	}
	if (ypos2 < 30) {
		return false;
	}

	float xpos = lines[0][CENTER_X] / lines[0][MAGNITUDE];
	float angle = lines[0][THETA] / lines[0][MAGNITUDE];
	//float ypos = lines[0][2] / lines[0][3];
	int error = ((xpos - (COLS / 2)) / (COLS / 2)) * 255;
	pic_com[1] = (unsigned char)abs(error);
	if (angle < 0) {
		pic_com[2] = (unsigned char)(-angle * 128 / (PI / 2));
	}
	else
		pic_com[2] = (unsigned char)(255 - angle * 128 / (PI / 2));

	if (fabs(xpos1 - xpos2) < T_THRESH) {// This is a T
		pic_com[0] = FOUND_T;
		return true;
	}
	if (xpos1 > xpos2) { // This is a corner left turn
		pic_com[0] = FULL_LEFT_TURN;
		return true;
	}
	else { // This is a corner right turn
		pic_com[0] = FULL_RIGHT_TURN;
		return true;
	}
}
