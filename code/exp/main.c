#include <stdio.h>

int main()
{
	unsigned short s;
	char hehe[] = "65536";
	int a;

	a = atoi(hehe);
	s = a;
	printf("s = %x\n", s);

	return 0;
}
