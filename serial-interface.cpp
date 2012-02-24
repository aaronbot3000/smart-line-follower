#include "serial-interface.hpp"

static int fd;
static char ibuf[128];
static char audiobuf[128];
static struct termios s_options;

char* open_serial() {
	fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(fd, F_SETFL, O_NONBLOCK); // set non-blocking reads, return 0 on no chars avail.

	tcgetattr(fd, &s_options);

	s_options.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	s_options.c_cflag |= (CLOCAL | CREAD | CS8);

	s_options.c_lflag &= ~(ECHO | ECHOE | ICANON | ISIG);
	s_options.c_oflag &= ~ONLCR;

	cfsetispeed(&s_options, B19200);
	cfsetospeed(&s_options, B19200);

	tcsetattr(fd, TCSANOW, &s_options);

	return ibuf;
}

int read_serial() {
	memset(ibuf, 0, sizeof(ibuf));
	return read(fd, ibuf, sizeof(char) * sizeof(ibuf));
}

void write_serial(char* data) {
	write(fd, data, 3);
}

void playaudio(int filename) {
	sprintf(audiobuf, "/home/root/./playaudio.sh %.5d&", filename);
	system(audiobuf);
}

void send_start_message() {
	write(fd, "###", 6);
}

void send_fail_message() {
	write(fd, "$$$", 5);
}
