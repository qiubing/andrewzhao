#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int udp_bind(const char *addr, int port)
{
	int sockfd;
	struct sockaddr_in sa;
	unsigned int salen;

	salen = sizeof(sa);
	bzero(&sa, salen);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (addr != NULL) {
		if (inet_pton(AF_INET, addr, &sa.sin_addr) != 1) {
			perror(addr);
			exit(-1);
		}
	} else {
		sa.sin_addr.s_addr = INADDR_ANY;
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket");
		exit(-1);
	}

	if (bind(sockfd, (struct sockaddr *)&sa, salen) < 0) {
		perror("Bind");
		close(sockfd);
		exit(-1);
	}

	return sockfd;
}

int main(int argc, char *argv[])
{
	int fd;
	int port;
	int nrecv;
	char buf[128];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		return -1;
	}
	port = atoi(argv[1]);
	fd = udp_bind(NULL, port);

	while (1) {
		nrecv = recvfrom(fd, buf, sizeof(buf), 0, NULL, 0);
		if (nrecv < 0) {
			fprintf(stderr, "Recvfrom data failed\n");
		} else {
			fprintf(stdout, "%s\n", buf);
		}
	}

	close(fd);

	return 0;
}
