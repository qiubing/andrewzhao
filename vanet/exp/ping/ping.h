#include <signal.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1400

char sendbuf[BUFSIZE];

int datalen;
char *host;
int nsent = 0;
pid_t pid;
int sockfd;
int verbose;

void init_v6(void);
void proc_v6(char *, ssize_t, struct msghdr *, struct timeval *);
void send_v6(void);
void readloop(void);
void sig_alrm(int);
void tv_sub(struct timeval *, struct timeval *);

struct proto {
	void (*fproc) (char *, ssize_t, struct msghdr *, struct timeval *);
	void (*fsend) (void);
	void (*finit) (void);
	struct sockaddr *sasend;
	struct sockaddr *sarecv;
	socklen_t salen;
	int icmpproto;
} *pr;
