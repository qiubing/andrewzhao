#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#include "ipv6_cast.h"

char parsed_gps_info[BUFLEN];

void warning(const char *s)
{
	fprintf(stderr, "%s", s);
}

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

int gps_read_port(const int fd, char *buf, const int len)
{
	int nread;

	memset(buf, 0, len);
	nread = read(fd, buf, len);
	if (nread < 0)
		warning("Read gps port failed");

	return nread;
}

int main()
{
	char gpsinfo[BUFLEN], time[TIMELEN], lat[LATITUDELEN], lon[LONGITUDELEN];
	char ss[12], direction[8];
	float speed;
	double tmpf;
	int tmpi;
	int n, index, i, valid, len;
	int mark, pos, update = 0;
	int gps_fd, nread, CD_fd = -1, fail_count = 0;
	FILE *fp;
	int fd;

	fd = gps_open_port(GPSDEVICE);
	set_port_speed(fd, 9600);
	close(fd);

	fp = fopen(GPSDEVICE, "r");
	if (fp == NULL) {
		fprintf(stderr, "Open file failed\n");
		return -1;
	}

//	set_port_speed(fd, 9600);
	fprintf(stderr, "hehe\n");
	while(1) {
		if (fgets(gpsinfo, BUFLEN, fp) != NULL) {
			fprintf(stderr, "gpsinfo: %s", gpsinfo);
			mark = 0;
			pos = 0;
			for (i=0; gpsinfo[i] != '\0'; i++) {
				if (gpsinfo[i] == '$') {
					if (memcmp(gpsinfo+i, "$GPRMC", 6) == 0) {
						mark = 1;
						pos = i;
						break;
					} else {
						i += 6;
						continue;
					}
				} else {
					continue;
				}
			}

			if (mark == 0)
				continue;

			for (n=pos, index=0; gpsinfo[n]!='\n'; n++) {
				if (gpsinfo[n] == ',') {
					index++;
					switch(index) {
					case 1:
						memcpy(time, gpsinfo+n+1, TIMELEN-1);
						time[TIMELEN-1] = '\0';
						break;
					case 2:
						if (gpsinfo[n+1] == 'A') valid=1;
						else valid=0;
						break;
					case 3:
						memcpy(lat, gpsinfo+n+1, LATITUDELEN-3);
						lat[LATITUDELEN-3] = '\0';
						tmpf = strtod(lat, (char **) NULL);
						tmpi = ((int)tmpf)/100;
						tmpf = tmpf - tmpi*100;
						tmpf = tmpf/60;
						tmpf += tmpi;
						break;
					case 4:
						sprintf(lat, "%9.6f %c", tmpf, gpsinfo[n+1]);
						tmpf = 0;
						tmpi = 0;
						break;
					case 5:
						memcpy(lon, gpsinfo+n+1, LONGITUDELEN-3);
						lon[LONGITUDELEN-3] = '\0';
						tmpf = strtod(lon, (char **) NULL);
						tmpi = ((int)tmpf)/100;
						tmpf = tmpf - tmpi*100;
						tmpf = tmpf/60;
						tmpf += tmpi;
						break;
					case 6:
						sprintf(lon, "%10.6f %c", tmpf, gpsinfo[n+1]);
						break;
					case 7:
						for (i=n+1; gpsinfo[i]!=','; i++)
							;
						memcpy(ss, gpsinfo+n+1, i-n-1);
						ss[i-n-1] = '\0';
						#define KNTOKM 1.852
						speed = KNTOKM * strtof(ss, (char **) NULL);
						break;
					case 8:
						for (i=n+1; gpsinfo[i]!=','; i++)
							;
						memcpy(direction, gpsinfo+n+1, i-n-1);
						direction[i-n-n] = '\0';
					default:
						break;
					}
				}
			}
			if (!valid) {
				sprintf(parsed_gps_info, "N %s\n", time); // have no self-location info.
			} else {
				sprintf(parsed_gps_info, "V %s %s %s %.2f %s\n", time, lat, lon, speed, direction);
				fprintf(stderr, "%s", gpsinfo);
				update = 1;
			}
			fprintf(stderr, "%s\n", parsed_gps_info);
		} else {
			warning("GPS information read error, retrying...\n");
		}
	}

	fclose(fp);
	return 0;
}
