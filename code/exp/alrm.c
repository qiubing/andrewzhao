#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

static int sig_count = 5;

void sig_alrm(int signo)
{
	if (sig_count <= 0) {
		printf("%s: sig_count <= 0\n", __func__);
	} else {
		sig_count--;
		printf("%s: sig_count = %d\n", __func__, sig_count);
	}
}

int main()
{
	struct itimerval itval;

	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		printf("signal failed\n");
		return -1;
	}

	itval.it_value.tv_sec = 0;
	itval.it_value.tv_usec = 500000;
	itval.it_interval.tv_sec = 0;
	itval.it_interval.tv_usec = 500000;

	if (setitimer(ITIMER_REAL, &itval, NULL) < 0) {
		printf("setitimer failed\n");
		return -1;
	}

	pause();
	printf("After pause, sig_count = %d\n", sig_count);

	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	pause();
	printf("After pause, sig_count = %d\n", sig_count);
	return 0;
}
