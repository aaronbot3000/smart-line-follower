#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <stdio.h>

#define CAP_COLS 640
#define CAP_ROWS 480

#define COLS 320
#define ROWS 240

#define GAUSSIAN_SIZE  9
#define GAUSSIAN_SIGMA 3

#define CANNY_HIGH 70
#define CANNY_LOW  30

#define CANNY_APETURE 3

using namespace cv;

void init_camera();
void destroy_camera();

void grab_frame();
void process_frame();
Mat grab_processed_frame();

#endif
