#include <stdio.h>

int main()
{
	char dog[5] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee};
	char *p = &dog[1];
	unsigned long l = *(unsigned long *)p;
	int i;

	printf("dog len = %d\n", sizeof(dog));
	printf("dog=\t");
	for (i=0; i<5; i++)
		printf("%2x", dog[i]);
	printf("\n");
	printf("l=\t  %x\n", l);

	return 0;
}
