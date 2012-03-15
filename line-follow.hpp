#ifndef LINE_FOLLOW_HPP
#define LINE_FOLLOW_HPP

#include <opencv/highgui.h>

#include "camera.hpp"
#include "line-find.hpp"
#include "commands.hpp"
#include "serial-interface.hpp"

using namespace cv;

#define T_THRESH 20 //pixels

bool track_one_line(Vector<Scalar_<float> > lines);
// Returns false if we need to revert to one line tracking
bool track_two_lines(Vector<Scalar_<float> > lines);

#endif
