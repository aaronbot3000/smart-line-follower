#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>

#define COLS 160
#define ROWS 120

#define GAUSSIAN_SIZE  9
#define GAUSSIAN_SIGMA 9

#define THRESHOLD_LVL 170

#define CANNY_HIGH 70
#define CANNY_LOW  10

#define CANNY_APETURE 3

using namespace cv;

void init_camera();
void destroy_camera();

void grab_frame();
void process_frame();
Mat grab_processed_frame();

#endif
