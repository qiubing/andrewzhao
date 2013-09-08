#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

const char *gps_freq_cmd[3] = { "$PMTK000*32\r\n",
				"$PMTK251,115200*1F\r\n",
				"$PMTK300,100,0,0,0,0*2C\r\n" };

int set_port_speed(const int fd, const int speed)
{
	struct termios newtio,oldtio;

	if (tcgetattr(fd,&oldtio) != 0) { //tcgetattr get current termal in oldtio
		warning("Setup serial");
		return -1;
	}

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag |= CLOCAL|CREAD;
	newtio.c_cflag &=~CSIZE;
	newtio.c_cflag |= CS8;
	newtio.c_cflag &= ~PARENB;

	switch(speed) {
	case 9600:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;
	case 38400:
		cfsetispeed(&newtio,B38400);
		cfsetospeed(&newtio,B38400);
		break;
	case 115200:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;
	}
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 6;
	tcflush(fd, TCIFLUSH);
	if((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
		warning("Com set");
		return -1;
	}
	return 0;
}

int gps_open_port(const char *dev)
{
	int fd;

	fd = open(dev, O_RDWR);
	if(-1 == fd)
		warning("Can not open serial");

	return fd;
}

int gps_write_port(const int fd, const char *cmd, const int len)
{
	int nwrite;

	nwrite= write(fd, cmd, len);
	if (nwrite < 0)
		warning("Write gps port failed");

	return nwrite;
}

int gps_read_port(const int fd, char *buf, const int len)
{
	int nread;

	memset(buf, 0, len);
	nread = read(fd, buf, len);
	if (nread < 0)
		warning("Read gps port failed");

	return nread;
}

int gps_set_freq(const int fd)
{
	int i;
	int key = 0;
	char buf[BUFLEN_SHORT];

	if (set_port_speed(fd, 9600) < 0) {
		printf("set_port_speed error\n");
		return -1;
	}
	for (i=0; i<3; i++) {
		if (key == 0) {
			gps_write_port(fd, gps_freq_cmd[0], strlen(gps_freq_cmd[0]));
			gps_read_port(fd, buf, sizeof(buf));
			gps_read_port(fd, buf, sizeof(buf));
			key++;
		}
		if (key == 1) {
			gps_write_port(fd, gps_freq_cmd[1], strlen(gps_freq_cmd[1]));
			gps_read_port(fd, buf, sizeof(buf));
			gps_read_port(fd, buf, sizeof(buf));
			gps_read_port(fd, buf, sizeof(buf));
			if (set_port_speed(fd, 115200) < 0) {
				printf("set_port_spedd error\n");
				return -1;
			}
			gps_read_port(fd, buf, sizeof(buf));
			gps_read_port(fd, buf, sizeof(buf));
			key++;
		}
		if (key == 2) {
			gps_write_port(fd, gps_freq_cmd[2], strlen(gps_freq_cmd[2]));
			usleep(50);
			gps_read_port(fd, buf, sizeof(buf));
			gps_read_port(fd, buf, sizeof(buf));
		}
	}

	return 0;
}

