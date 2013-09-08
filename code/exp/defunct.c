#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn()
{

	fprintf(stderr, "%s\n", __func__);

	sleep(10);
	fprintf(stderr, "%s out\n", __func__);
	return ((void *)0);
}

int main()
{
	pthread_t tid;

	pthread_create(&tid, NULL, thr_fn, NULL);

	fprintf(stderr, "%s after generate thread..\n", __func__);

	return 0;
}
