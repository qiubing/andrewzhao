#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <net/if.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sockfd;
	char sendbuf[1024];
	struct sockaddr_in6 sa;
	struct msghdr msg;
	struct iovec iov;
	unsigned short port;
	char ipaddr[32];
	char devname[32] = "ath5k0";
	unsigned int outif;

	if (argc != 3) {
		printf("Usage: %s destaddr destport\n", argv[0]);
		return -1;
	}
	port = atoi(argv[2]);
	printf("Dest address is %s, port is %u\n", argv[1], port);
	sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		printf("Socket failed\n");
		return -1;
	}
	if ((outif = if_nametoindex(devname)) == 0) {
		printf("Parse devname %s to ifindex failed\n", devname);
		close(sockfd);
		return -1;
	}
	printf("Output ifindex = %u\n", outif);

	memset(&sa, 0, sizeof(sa));
	sa.sin6_family = AF_INET6;
	sa.sin6_port = htons(port);
	sa.sin6_scope_id = outif;
	if (inet_pton(AF_INET6, argv[1], &sa.sin6_addr) != 1) {
		printf("Assign destIP addr failed\n");
		return -1;
	}
	iov.iov_base = sendbuf;
	iov.iov_len = sizeof(sendbuf);
	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (struct sockaddr *)&sa;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_namelen = sizeof(sa);

	memset(sendbuf, 0, sizeof(sendbuf));
	sprintf(sendbuf, "vanet0123456789\n");
	iov.iov_len = strlen(sendbuf);
	if (sendmsg(sockfd, &msg, 0x20000) < 0) {
		printf("Send information failed\n");
		return -1;
	}
	close(sockfd);

	return 0;
}
