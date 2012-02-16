#include "line-find.hpp"

static vector<vector<Point> > contours;
static vector<vector<Point> > lines;
static vector<vector<Point> >::iterator it;

static Mat outimg;

void init_outimg() {
	outimg = Mat(240, 320, CV_8UC1);
}

void get_lines(Mat input) {
	// clear old contours
	contours.clear();
	lines.clear();
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (it = contours.begin() ; it < contours.end(); it++) {
		lines.push_back(vector<Point>());
		approxPolyDP(*it, lines.back(), APPROX_EPSILON, false);
	}
}

Mat get_line_image() {
	Scalar color(255, 255, 255);
	Scalar black(0, 0, 0);
	outimg.setTo(black);
	drawContours(outimg, lines, -1, color);
	return outimg;
}


