#include <stdio.h>
#include <pthread.h>
#include <string.h>
#define NUM_THREADS 5


void *producer(void *param);
void *consumer(void *param);

int main(void)
{
	int i;
	int shared_var = -1;
	int numReaders = 0;

	pthread_t prods[NUM_THREADS];
	pthread_t cons[NUM_THREADS];

	pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER;
	pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;

	int prod_params[NUM_THREADS];

	for (i=0; i<NUM_THREADS; i++)
	{
		prod_params[i] = i
		if(pthread_create(&prods[i],NULL,producer,&prod_params[i]) != 0)
		{
			printf("Unable to create producer thread \n");
			exit(1);
		}
	}

	for (i=0; i<NUM_THREADS; i++)
	{
		if(pthread_create(&cons[i],NULL,consumer,NULL) != 0)
		{
			printf("Unable to create consumer thread \n");
			exit(1);
		}
	}

	for (i=0; i<NUM_THREADS; i++)
	{
		pthread_join(prods[i]);
		pthread_join(cons[i]);
	}
	return 0;
}

void *producer(void *param)
{
	int num = *(int *)param

	pthread_mutex_lock (&m);
		while(numReaders != 0)
		{
			pthread_cond_wait(&c_prod, &m);
		}
		shared_var = num;
		printf("Producer: Value written is: %d \n",num);
		printf("num of readers are: %d\n",numReaders);
	pthread_mutex_unlock(&m);

	pthread_cond_signal (&c_cons);
	printf("Producer inserted: %d",num);
	return 0;
}









