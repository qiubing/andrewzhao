#include <unistd.h>
#include <stdio.h>

int main()
{
	int abcdefg = 6;

	printf("abcdefg<%x>\n", &abcdefg);
	while (abcdefg >0) {
		abcdefg--;
		sleep(10);
	}

	return 0;
}
