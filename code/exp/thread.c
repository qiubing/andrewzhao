#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include <pthread.h>

void printids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg)
{
	printids("new thread: ");
	sleep(1);
	printf("thr_fn exit\n");

	return ((void *)0);
}

void gen_thread()
{
	int err;
	pthread_t tid;

	err = pthread_create(&tid, NULL, thr_fn, NULL);
	if (err != 0)
		printf("%s\n", strerror(err));
	err = pthread_detach(tid);
	if (err != 0)
		printf("%s\n", strerror(err));
	printf("%s exit\n", __func__);
}

int main()
{
	int i;

	for (i=0; i<5; i++) {
		gen_thread();
		sleep(10);
	}

	printf("exit\n");

	return 0;
}
