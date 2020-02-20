#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *threadFnc (void *pArg)
{
	int num = *(int *)pArg;
	printf("Hello thread %d\n",num);
	return 0;
}

int main(void)
{
	int i;
	pthread_t tid[NUM_THREADS];
	int tNum[NUM_THREADS];

	for (i=0; i<NUM_THREADS; i++)
	{
		tNum[i] = i;
		pthread_create(&tid[i],NULL,threadFnc,&tNum[i]);
	}

	for (i=0; i<NUM_THREADS; i++)
	{
		pthread_join(tid[i],NULL);
	}
	return 0;
}