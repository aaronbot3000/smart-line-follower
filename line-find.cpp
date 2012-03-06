#include "line-find.hpp"

static vector<vector<Point> > contours;
static vector<Scalar_<float> > clean_lines;
static vector<Scalar_<float> > really_clean_lines;

static vector<Point> currentline;

#ifdef __x86_64
static vector<vector<Point> > lines;
#endif

vector<Scalar_<float> > get_lines(Mat input) {
	// clear old contours and lines
	contours.clear();
	clean_lines.clear();
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	vector<vector<Point> >::iterator it;

#ifdef __x86_64
	lines.clear();
	for (it = contours.begin() ; it < contours.end(); it++) {
		lines.push_back(vector<Point>());
		approxPolyDP(*it, lines.back(), APPROX_EPSILON, true);
	}
#endif

	for (it = contours.begin(); it < contours.end(); it++) {
		currentline.clear();
		approxPolyDP(*it, currentline, APPROX_EPSILON, true);

		int prevx = -100, prevy = -100;
		vector<Point>::iterator itl;
		for (itl = currentline.begin(); itl < currentline.end(); itl++) {
			// If first point
			if (prevx < 0) {
				prevx = (*itl).x;
				prevy = (*itl).y;
				continue;
			}

			int dx = (*itl).x - prevx;
			int dy = (*itl).y - prevy;
			int xc = (*itl).x - dx / 2;
			int yc = (*itl).y - dy / 2;
			prevx = (*itl).x;
			prevy = (*itl).y;
				
			// Restrict range to +- PI/2
			if (dx < 0) {
				dx = -dx;
				dy = -dy;
			}

			float theta = atan2(dy, dx);
			float magn  = sqrt(dy * dy + dx * dx);
			
			if (magn < MIN_MAGN)
				continue;

			if (clean_lines.empty()) {
				Scalar newpoint(theta * magn,
						xc * magn,
						yc * magn,
						magn);
				clean_lines.push_back(newpoint);
				continue;
				//printf("%f, at %d %d length %f\n", theta, xc, yc, magn);
			}

			vector<Scalar_<float> >::iterator itt = clean_lines.begin();
			bool found = 0;
			for (; itt < clean_lines.end(); itt++) {
				float testtheta = (*itt)[THETA] / (*itt)[MAGNITUDE];
				if (fabs(testtheta - theta) < THETA_THRESHOLD) {
					found = 1;
					break;
				}
				if (fabs(testtheta - (theta + PI)) < THETA_THRESHOLD) {
					theta += PI;
					found = 1;
					break;
				}
				if (fabs(testtheta - (theta - PI)) < THETA_THRESHOLD) {
					theta -= PI;
					found = 1;
					break;
				}
			}
			if (found) {
				(*itt)[THETA] += theta * magn;
				(*itt)[CENTER_X] += xc * magn;
				(*itt)[CENTER_Y] += yc * magn;
				(*itt)[MAGNITUDE] += magn;
			}
			else {
				Scalar newpoint(theta * magn,
						xc * magn,
						yc * magn,
						magn);
				clean_lines.push_back(newpoint);
				//printf("%f, at %d %d length %f\n", theta, xc, yc, magn);
			}
		}
	}

	collect_similar_lines();
	return really_clean_lines;
}

void collect_similar_lines() {
	really_clean_lines.clear();

	vector<Scalar_<float> >::iterator it1 = clean_lines.begin();
	for (; it1 < clean_lines.end(); it1++) {
		Scalar_<float> line = (*it1);

		if (really_clean_lines.empty()) {
			really_clean_lines.push_back(line);
			continue;
		}

		float theta = line[THETA]/line[MAGNITUDE];

		bool found = 0;
		vector<Scalar_<float> >::iterator itr;
		for (itr = really_clean_lines.begin(); itr < really_clean_lines.end(); itr++) {
			float testtheta = (*itr)[THETA] / (*itr)[MAGNITUDE];

			if (fabs(testtheta - theta) < THETA_THRESHOLD) {
				found = 1;
				break;
			}
			if (fabs(testtheta - (theta + PI)) < THETA_THRESHOLD) {
				theta += PI;
				found = 1;
				break;
			}
			if (fabs(testtheta - (theta - PI)) < THETA_THRESHOLD) {
				theta -= PI;
				found = 1;
				break;
			}
		}
		if (found) {
			(*itr)[THETA] += theta * line[MAGNITUDE];
			(*itr)[CENTER_X] += line[CENTER_X];
			(*itr)[CENTER_Y] += line[CENTER_Y];
			(*itr)[MAGNITUDE] += line[MAGNITUDE];
		}
		else {
			really_clean_lines.push_back(line);
			//printf("%f, at %d %d length %f\n", theta, xc, yc, magn);
		}
	}

	Scalar_<float> top1;
	Scalar_<float> top2;
	// Pick out the two strongest lines
	if (really_clean_lines.size() > 2) {
		it1 = really_clean_lines.begin();
		top1 = *it1;
		it1++;
		top2 = *it1;
		it1++;

		if (top2[MAGNITUDE] > top1[MAGNITUDE]) {
			Scalar_<float> temp = top1;
			top1 = top2;
			top2 = temp;
		}

		for (; it1 < really_clean_lines.end(); it1++) {
			if ((*it1)[MAGNITUDE] > top1[MAGNITUDE]) {
				top2 = top1;
				top1 = (*it1);
			}
			else if ((*it1)[MAGNITUDE] > top2[MAGNITUDE]) {
				top2 = (*it1);
			}
		}
		really_clean_lines.push_back(top1);
		really_clean_lines.push_back(top2);
	}
	
	if (really_clean_lines.size() > 1) {
		// Sort the lines based on theta
		top1 = really_clean_lines[0];
		top2 = really_clean_lines[1];


		really_clean_lines.clear();
		if (fabs(top1[THETA] / top1[MAGNITUDE]) > fabs(top2[THETA] / top2[MAGNITUDE])) {
			really_clean_lines.push_back(top1);
			really_clean_lines.push_back(top2);
		}
		else {
			really_clean_lines.push_back(top2);
			really_clean_lines.push_back(top1);
		}
	}
}

#ifdef __x86_64
void get_line_image(Mat i) {
	const Scalar color(255, 255, 255);
	const Scalar black(0, 0, 0);
	i.setTo(black);
	drawContours(i, lines, -1, color);
}

void draw_found_lines(Mat i) {
	const Scalar color(0, 255, 0);
	printf("size %lu\n", really_clean_lines.size());
	vector<Scalar_<float> >::iterator itt = really_clean_lines.begin();
	for (; itt < really_clean_lines.end(); itt++) {
		float theta = (*itt)[THETA] / (*itt)[MAGNITUDE];
		float xc    = (*itt)[CENTER_X] / (*itt)[MAGNITUDE];
		float yc    = (*itt)[CENTER_Y] / (*itt)[MAGNITUDE];
		Point start(xc - 100 * cos(theta), yc - 100 * sin(theta));
		Point end(xc + 100 * cos(theta), yc + 100 * sin(theta));
		line(i, start, end, color);
		//printf("%f, at %d %d to %d %d\n", theta, start.x, start.y, end.x, end.y);
	}
}
	
#endif
