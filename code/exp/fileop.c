#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	FILE *fp;

	printf("My PID is %d\n", getpid());

	fp = fopen("/var/log/vanet/20120518.sndres", "a");
	if (fp == NULL) {
		perror("Open file error");
		exit(-1);
	}

	fclose(fp);
	return 0;
}
