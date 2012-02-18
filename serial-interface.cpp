#include "serial-interface.hpp"

static int fd;
static char ibuf[128];
static char obuf[128];
static char audiobuf[128];
static struct termios s_options;

void open_serial() {
	fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(fd, F_SETFL, O_NONBLOCK); // set non-blocking reads, return 0 on no chars avail.

	tcgetattr(fd, &s_options);

	s_options.c_cflag &= ~(PARENB | CSTOPB | CSIZE);
	s_options.c_cflag |= (CLOCAL | CREAD | CS8);

	s_options.c_lflag &= ~(ECHO | ECHOE);
	s_options.c_lflag |= ICANON;

	s_options.c_oflag &= ~ONLCR;

	cfsetispeed(&s_options, B9600);
	cfsetospeed(&s_options, B9600);

	tcsetattr(fd, TCSANOW, &s_options);
}

void read_serial() {
	int num_bytes;
	num_bytes = read(fd, ibuf, sizeof(char) * sizeof(ibuf));
	if (num_bytes < 0)
		return;

	memset(ibuf, 0, sizeof(ibuf));
}

void write_serial(char* data) {
	sprintf(obuf, "%s\n", data);
	printf("%s\n", obuf);
	write(fd, obuf, 4);
}

void playaudio(int filename) {
	sprintf(audiobuf, "/home/root/./playaudio.sh %.5d&", filename);
	system(audiobuf);
}

void send_start_message() {
	sprintf(obuf, "\nboob\n");
	write(fd, obuf, 6);
}

void send_fail_message() {
	sprintf(obuf, "\nfuuu\n");
	write(fd, obuf, 5);
}
