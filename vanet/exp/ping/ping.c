#include "ping.h"

void error(const char *msg)
{
	perror(msg);
	exit(-1);
}

struct proto proto_v6 = {
	proc_v6,
	send_v6,
	init_v6,
	NULL,
	NULL,
	0,
	IPPROTO_ICMPV6,
};

int datalen = 56;

void tv_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0) {
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

void init_v6()
{
	int on = 1;
	if (verbose == 0) {
		struct icmp6_filter myfilt;
		ICMP6_FILTER_SETBLOCKALL(&myfilt);
		ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &myfilt);
		setsockopt(sockfd, IPPROTO_IPV6, ICMP6_FILTER, &myfilt, sizeof(myfilt));
	}
	setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVHOPLIMIT, &on, sizeof(on));
}

void sig_alrm(int signo)
{
	(*pr->fsend)();

	alarm(5);
	return;
}

void proc_v6(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv)
{
	double rtt;
	struct icmp6_hdr *icmp6;
	struct timeval *tvsend;
	struct cmsghdr *cmsg;
	int hlim;
	char addrbuf[BUFSIZE];

	icmp6 = (struct icmp6_hdr *) ptr;
	if (len < 8)
		return;

	if (icmp6->icmp6_type == ICMP6_ECHO_REPLY) {
		if (icmp6->icmp6_id != pid)
			return;
		if (len < 16)
			return;

		tvsend = (struct timeval *) (icmp6 + 1);
		tv_sub(tvrecv, tvsend);
		rtt = tvrecv->tv_sec * 1000.0 + tvrecv->tv_usec / 1000.0;

		hlim = -1;
		for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg)) {
			if (cmsg->cmsg_level == IPPROTO_IPV6 && cmsg->cmsg_type == IPV6_HOPLIMIT) {
				hlim = *(u_int32_t *) CMSG_DATA(cmsg);
				break;
			}
		}
		printf("%d bytes from <%s>: seq=%u, hlim=", len,
				inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)(pr->sarecv))->sin6_addr), addrbuf, pr->salen),
				icmp6->icmp6_seq);
		if (hlim == -1)
			printf("???");
		else
			printf("%d", hlim);
		printf(", rtt=%.3f ms\n", rtt);
	} else if (verbose) {
		printf(" %d bytes from <%s>: type=%d, code=%d\n",
			len, inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)(pr->sarecv))->sin6_addr), addrbuf, pr->salen),
			icmp6->icmp6_type, icmp6->icmp6_code);
	}
}

void send_v6()
{
	int len;
	struct icmp6_hdr *icmp6;

	icmp6 = (struct icmp6_hdr *) sendbuf;
	icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
	icmp6->icmp6_code = 0;
	icmp6->icmp6_id = pid;
	icmp6->icmp6_seq = nsent++;
	memset((icmp6 + 1), 0xa5, datalen);
	gettimeofday((struct timeval *)(icmp6 + 1), NULL);

	len = 8 + datalen;
	if (sendto(sockfd, sendbuf, len, 0, pr->sasend, pr->salen) != len)
		error("Sendto");
}

void readloop(void)
{
	int size;
	char recvbuf[BUFSIZE];
	char controlbuf[BUFSIZE];
	struct msghdr msg;
	struct iovec iov;
	ssize_t n;
	struct timeval tval;

	sockfd = socket(AF_INET6, SOCK_RAW, pr->icmpproto);
	if (sockfd < 0)
		error("Socket");
	setuid(getuid());

	if (pr->finit)
		(*pr->finit) ();

	size = 60*1024;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	
	sig_alrm(SIGALRM);

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;

	for (;;) {
		msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockfd, &msg, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				error("Recvmsg");
		}
		gettimeofday(&tval, NULL);
		(*pr->fproc) (recvbuf, n, &msg, &tval);
	}
}

int main(int argc, char *argv[])
{
	int c;
	char *ipv6addr;
	struct sockaddr_in6 srcsa, dstsa;
	socklen_t salen;

	if (1 == argc) {
		fprintf(stderr, "Usage: %s [-v] IPv6Address\n", argv[0]);
		return 0;
	}
	if (2 == argc) {
		verbose = 0;
		ipv6addr = argv[1];
	} else {
		verbose = 1;
		ipv6addr = argv[2];
	}

	pid = getpid() & 0xffff;
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		error("can not catch SIGALRM");

	printf("PING <%s>: %d data bytes\n", ipv6addr, datalen);

	salen = sizeof(dstsa);
	bzero(&srcsa, salen);
	bzero(&dstsa, salen);
	dstsa.sin6_family = AF_INET6;
	if (inet_pton(AF_INET6, ipv6addr, &dstsa.sin6_addr) != 1)
		error("Assign destIP addr");

	pr = &proto_v6;
	pr->sasend = (struct sockaddr *)&dstsa;
	pr->sarecv = (struct sockaddr *)&srcsa;
	pr->salen = salen;

	readloop();

	return 0;
}
