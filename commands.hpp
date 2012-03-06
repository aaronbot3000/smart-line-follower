#ifndef COMMANDS_HPP
#define COMMANDS_HPP

// Sent to the PIC
#define LEFT 'd'
#define RIGHT 'a'
#define CONTINUE 'i'
#define FULL_LEFT_TURN 'j'
#define FULL_RIGHT_TURN 'l'
#define FOUND_T 't'

// Received from the PIC
#define GET_LINE_DATA 'a'
#define NUM_LINES_FOUND 'b'

bool track_one_line(Vector<Scalar_<float> > lines);
// Returns false if we need to revert to one line tracking
bool track_two_lines(Vector<Scalar_<float> > lines);

#endif
