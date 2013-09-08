#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	FILE *fp;
	char buf[1024], rbuf[1024];

	fp = fopen("/dev/ttyUSB0", "r");
	setvbuf(fp, buf, _IONBF, sizeof(buf));
	fflush(fp);
	while(1) {
		fgets(rbuf, sizeof(rbuf), fp);
		fprintf(stderr, "%s", rbuf);
		bzero(rbuf, sizeof(rbuf));
	}

	fclose(fp);

	return 0;
}
