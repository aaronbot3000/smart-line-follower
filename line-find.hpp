#ifndef LINE_FIND_HPP
#define LINE_FIND_HPP

#include <opencv/cv.h>
#include <vector>
#include <iostream>

#include "camera.hpp"

#define APPROX_EPSILON 10
#define PI 3.14159265

#define THETA_THRESHOLD PI / 8
#define MIN_MAGN 10

using namespace cv;
using namespace std;

void get_lines(Mat input);
void collect_similar_lines();

#ifdef __x86_64
void get_line_image(Mat i);
void draw_found_lines(Mat i);
#endif

#endif
