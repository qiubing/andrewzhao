#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFLEN 128
#define TIMELEN 10
#define LATLEN 10
#define LONLEN 11
#define KNTOKM 1.852

int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[BUFLEN], buf2[BUFLEN], ss[10];
	char time[TIMELEN];
	char lat[LATLEN];
	char lon[LONLEN];
	int n, i;
	int index;
	int valid;
	float speed;

	if (argc != 2) {
		printf("Please input file path\n");
		return -1;
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("Cannot open file\n");
		return -1;
	}
	while (fgets(buf, BUFLEN, fp) != NULL) {
		if (memcmp(buf, "$GPRMC", 6) == 0) {
			for (n=0, index=0; buf[n]!='\n'; n++) {
				if (buf[n] == ',') {
					index++;
					if (index == 1) { // UTC time
						memcpy(time, buf+n+1, TIMELEN);
					} else if (index == 2){ //Located?
						if (buf[n+1] == 'A')
							valid = 1;
						else
							valid = 0;
					} else if (index == 3) { // Latitude
						memcpy(lat, buf+n+1, LATLEN-1);
					} else if (index == 4) { // Latitude N or S
						memcpy(lat+LATLEN-1, buf+n+1, 1);
					} else if (index == 5) { // Longitude
						memcpy(lon, buf+n+1, LONLEN-1);
					} else if (index == 6) { // Longitude E or W
						memcpy(lon+LONLEN-1, buf+n+1, 1);
					} else if (index == 7) { // Speed in Knots
						for (i=n+1; buf[i]!=','; i++)
							;
						memcpy(ss, buf+n+1, i-n-1);
						ss[i-n-1] = '\0';
						speed = KNTOKM * strtof(ss, (char **) NULL);
					} else {
						break;
					}
				}
			}
			printf("read: %s", buf);
			if (valid == 1) {
				memcpy(buf2, time, TIMELEN);
				buf2[TIMELEN] = '\0';
				printf("\tTIME: %s\n", buf2);
				memcpy(buf2, lat, LATLEN);
				buf2[LATLEN] = '\0';
				printf("\tLATI: %s\n", buf2);
				memcpy(buf2, lon, LONLEN);
				buf2[LONLEN] = '\0';
				printf("\tLONG: %s\n", buf2);
				printf("\tSPEED: %6.2fkm/h\n", speed);
			}
		}
	}

	printf("Get out\n");
	fclose(fp);

	return 0;
}
