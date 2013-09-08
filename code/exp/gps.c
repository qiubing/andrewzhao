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

int main()
{
	char gpsinfo[BUFLEN], time[TIMELEN], lat[LATITUDELEN], lon[LONGITUDELEN];
	char buf[BUFLEN];
	char ss[12], direction[8];
	float speed;
	double tmpf;
	int tmpi;
	int n, index, i, valid, len;
	int mark, pos, update = 0;
	int gps_fd, nread, CD_fd = -1, fail_count = 0;
	FILE *fp;

	fp = fopen(GPSDEVICE, "r");
	if (fp == NULL) {
		fprintf(stderr, "Open file failed\n");
		return -1;
	}
	bzero(buf, sizeof(buf));
	if (setvbuf(fp, buf, _IOLBF, sizeof(buf)) != 0) {
		fprintf(stderr, "Set buffer failed\n");
		fclose(fp);
		return -1;
	}

	while(1) {
		memset(gpsinfo, 0, sizeof(gpsinfo));
		if (fgets(gpsinfo, sizeof(gpsinfo), fp) != NULL) {
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
						memcpy(lat, gpsinfo+n+1, LATITUDELEN-2);
						lat[LATITUDELEN-2] = '\0';
						tmpf = strtod(lat, (char **) NULL);
						tmpi = ((int)tmpf)/100;
						tmpf = tmpf - tmpi*100;
						tmpf = tmpf/60;
						tmpf += tmpi;
						break;
					case 4:
						sprintf(lat, "%7.4f %c", tmpf, gpsinfo[n+1]);
						tmpf = 0;
						tmpi = 0;
						break;
					case 5:
						memcpy(lon, gpsinfo+n+1, LONGITUDELEN-2);
						lon[LONGITUDELEN-2] = '\0';
						tmpf = strtod(lon, (char **) NULL);
						tmpi = ((int)tmpf)/100;
						tmpf = tmpf - tmpi*100;
						tmpf = tmpf/60;
						tmpf += tmpi;
						break;
					case 6:
						sprintf(lon, "%8.4f %c", tmpf, gpsinfo[n+1]);
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
				fprintf(stderr, "%s\n", gpsinfo);
				update = 1;
			}
			fprintf(stderr, "%s", parsed_gps_info);
		} else {
			warning("GPS information read error, retrying...\n");
		//	strcpy(parsed_gps_info, VANET_DEFAULT_MSG);
		//	sleep(GPS_REOPEN_TIME); //wait for GPS device
		//	if ((fp = fopen(GPSDEVICE, "r")) == NULL) {
		//		warning("Open GPS device path failed");
		//	}
		}
	}

	fclose(fp);
	return 0;
}
