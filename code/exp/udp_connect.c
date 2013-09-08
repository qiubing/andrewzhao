#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int udp_connect(const char *addr, int port)
{
	int sockfd;
	struct sockaddr_in sa;
	unsigned int salen;

	salen = sizeof(sa);
	bzero(&sa, salen);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (inet_pton(AF_INET, addr, &sa.sin_addr) != 1) {
		perror(addr);
		exit(-1);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket");
		exit(-1);
	}

	if (connect(sockfd, (struct sockaddr *)&sa, salen) < 0) {
		perror("Connect");
		close(sockfd);
		exit(-1);
	}

	return sockfd;
}

int main(int argc, char *argv[])
{
	int fd;
	int port;
	int nsend;
	int count = 0;
	char buf[128];

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <address> <port>\n", argv[0]);
		return -1;
	}
	port = atoi(argv[2]);
	fd = udp_connect(argv[1], port);

	while (1) {
		sprintf(buf, "[%3d] %s", count, "hello");
		nsend = sendto(fd, buf, strlen(buf), MSG_DONTWAIT, NULL, 0);
		if (nsend < strlen(buf)) {
			fprintf(stderr, "Sendto data failed\n");
		}
		count++;
		sleep(1);
	}

	close(fd);

	return 0;
}
