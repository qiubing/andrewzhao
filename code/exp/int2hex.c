#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int temp;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s num\n", argv[0]);
		exit (-1);
	}
	temp = atoi(argv[1]);
	printf("%s = 0x%x\n", argv[1], temp);

	return 0;
}
