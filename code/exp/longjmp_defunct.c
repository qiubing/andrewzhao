#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

static jmp_buf bw_test_jmp_buf;
void sig_alrm(int signo)
{
	if (signo != SIGALRM) {
		perror("signo not SIGALRM");
		return ;
	}
	longjmp(bw_test_jmp_buf, 1);
}

void *thr_bw_test()
{
	struct itimerval itval;
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("Signal");
		return ((void *)0);
	}
	itval.it_value.tv_sec = 5;
	itval.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itval, NULL);
	if (setjmp(bw_test_jmp_buf) != 0) {
		fprintf(stderr, "Recv timeout or finished\n");
	} else {
		sleep(100);
	}
	itval.it_value.tv_sec = 0;
	itval.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &itval, NULL);

	fprintf(stderr, "%s return\n", __func__);
	return ((void *)0);
}

int bw_test()
{
	pthread_t tid;

	pthread_create(&tid, NULL, thr_bw_test, NULL);
	pthread_detach(tid);

	fprintf(stdout, "Band test thread generated...\n");
	return 0;
}

int main()
{
	bw_test();
	sleep(1000);
	return 0;
}
