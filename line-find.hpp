#ifndef LINE_FIND_HPP
#define LINE_FIND_HPP

#include <opencv/cv.h>
#include <vector>
#include <iostream>

#include "camera.hpp"

#define APPROX_EPSILON 10

using namespace cv;
using namespace std;

void get_lines(Mat input);

#ifdef __x86_64
void init_outimg();
Mat get_line_image();
#endif

#endif
