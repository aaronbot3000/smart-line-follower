#include "line-find.hpp"

static vector<vector<Point> > contours;
static vector<Scalar_<float> > clean_lines;

static vector<Point> currentline;

#ifdef __x86_64
static vector<vector<Point> > lines;
#endif

void get_lines(Mat input) {
	// clear old contours and lines
	contours.clear();
	clean_lines.clear();
	findContours(input, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

#ifdef __x86_64
	lines.clear();
	vector<vector<Point> >::iterator it;
	for (it = contours.begin() ; it < contours.end(); it++) {
		lines.push_back(vector<Point>());
		approxPolyDP(*it, lines.back(), APPROX_EPSILON, false);
	}
#endif

	for (it = contours.begin(); it < contours.end(); it++) {
		currentline.clear();
		approxPolyDP(*it, currentline, APPROX_EPSILON, false);

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
				//printf("%f, at %d %d length %f\n", theta, xc, yc, magn);
			}
			else {
				vector<Scalar_<float> >::iterator itt = clean_lines.begin();
				bool found = 0;
				for (; itt < clean_lines.end(); itt++) {
					float testtheta = (*itt)[0] / (*itt)[3];
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
					(*itt)[0] += theta * magn;
					(*itt)[1] += xc * magn;
					(*itt)[2] += yc * magn;
					(*itt)[3] += magn;
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
	vector<Scalar_<float> >::iterator itt = clean_lines.begin();
	for (; itt < clean_lines.end(); itt++) {
		float theta = (*itt)[0] / (*itt)[3];
		float xc    = (*itt)[1] / (*itt)[3];
		float yc    = (*itt)[2] / (*itt)[3];
		Point start(xc - 100 * cos(theta), yc - 100 * sin(theta));
		Point end(xc + 100 * cos(theta), yc + 100 * sin(theta));
		line(i, start, end, color);
		printf("%f, at %d %d to %d %d\n", theta, start.x, start.y, end.x, end.y);
	}
}
	
#endif
