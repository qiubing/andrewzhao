#include<stdio.h>

struct ipv6hdr {
	unsigned char b:4,
		      a:4;
};

int main()
{
	unsigned int u32 = 0x11223344;
	unsigned char *u8p = &u32;
	struct ipv6hdr hdr;
	int i;

	hdr.a = 0xa;
	hdr.b = 0xb;
	printf("u32\t%p:\t%x\n", &u32, u32);
	for (i=0; i<4; i++)
		printf("\t%p:\t%x\n", u8p+i, *(u8p+i));
	u8p = &hdr;
	printf("hdr: %x\n", *u8p);

	return 0;
}
