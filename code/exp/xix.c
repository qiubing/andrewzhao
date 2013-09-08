#include <stdio.h>
#include <unistd.h>

int main()
{
	int i;
	int array[10];

	for (i=0; i<=10; i++) {
		printf("i = %d, array[i]<%p>, i<%p>\n", i, &array[i], &i);
		array[i] = 0;
		printf("i = %d, array[i]<%p>, i<%p>\n", i, &array[i], &i);
		sleep(1);
	}

	return 0;
}
