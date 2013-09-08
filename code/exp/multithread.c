#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	int i;
	int times;
	if (argc == 2) {
		times = argv[1][0] - '0';
	} else
		times = 1;
	printf("times = %d\n", times);
	for(i=0; i<times; i++)
		fork();
	sleep(10);
	printf("I am exit\n");
	return 0;
}
