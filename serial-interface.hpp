#ifndef SERIAL_INTERFACE_H
#define SERIAL_INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>

#define SERIAL_PORT "/dev/ttyUSB0"

char* open_serial();
int read_serial();
void write_serial(char* data);

void send_start_message();

void playaudio(int filename);

#endif
