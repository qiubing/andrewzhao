#include <stdio.h>

int main()
{
	unsigned int a = 0xfffffff7;
	unsigned char *b = (unsigned char *)&a;
	unsigned int c = 0x112233f4;
	char *d = (char *)&c;
	int e = -1;
	unsigned int *f = (unsigned int *)&e;
	char g = -1;
	char h = -2;
	char i = -3;

	printf("%x, %x, %x, %x\n", *b, *d, *f, g);
	printf("g<%p>, h<%p>, i<%p>\n", &g, &h, &i);

	return 0;
}
