#include <stdio.h>

int main()
{
	unsigned short a[4], h = 0;
	int i;

	for (i=0; i<4; i++)
		scanf("%x", &a[i]);
	for (i=0; i<4; i++)
		printf("%x\n", a[i]);

	h = a[0] ^ a[1] ^ a[2] ^ a[3];
	printf("h = 0x%x\n", h);

	return 0;
}
