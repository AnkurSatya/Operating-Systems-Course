#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 5

int shared_var = 0;
int num_readers = 0;
int readers_wait = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;

void *producer(void *param);
void *consumer(void *param);

int main(void)
{
	int i;

	pthread_t prods[NUM_THREADS];
	pthread_t cons[NUM_THREADS];

	int prod_params[NUM_THREADS];
	int cons_params[NUM_THREADS];

	for (i=0; i<NUM_THREADS; i++)
	{
		cons_params[i] = i;
		if(pthread_create(&cons[i],NULL,consumer,&cons_params[i]) != 0)
		{
			printf("Unable to create consumer thread \n");
			exit(1);
		}
	}

	for (i=0; i<NUM_THREADS; i++)
	{
		prod_params[i] = i;
		if(pthread_create(&prods[i],NULL,producer,&prod_params[i]) != 0)
		{
			printf("Unable to create producer thread \n");
			exit(1);
		}
	}


	for (i=0; i<NUM_THREADS; i++)
	{
		pthread_join(prods[i],NULL);
		pthread_join(cons[i],NULL);
	}
	return 0;
}

void *consumer(void *param)
{
	int num = *(int *)param;
	int tmp_num_readers = 0;
	
	
	pthread_mutex_lock(&m);
		readers_wait+=1;
		while(num_readers == -1)
		{
			pthread_cond_wait(&c_cons, &m);
		}
		readers_wait-=1;
		tmp_num_readers = ++num_readers;
	pthread_mutex_unlock(&m);
	printf("Reader number, value read, readers waiting: %d %d %d \n",num,shared_var,tmp_num_readers);

	pthread_mutex_lock(&m);
		num_readers--;
		if(num_readers == 0)
		{
			pthread_cond_signal (&c_prod);
		}
	pthread_mutex_unlock(&m);
	return 0;
}

void *producer(void *param)
{
	int num = *(int *)param;
	int tmp_shared_num;
	int tmp_num_readers;

	pthread_mutex_lock (&m);
		while(num_readers != 0)
		{
			pthread_cond_wait(&c_prod, &m);
		}
		shared_var = num;
		tmp_shared_num = shared_var;
		num_readers = -1;
		tmp_num_readers = num_readers;
	pthread_mutex_unlock(&m);
	printf("Producer number, Value written, number of readers: %d %d %d \n",num, tmp_shared_num, tmp_num_readers);

	pthread_mutex_lock(&m);
		num_readers = 0;
		if(readers_wait > 0)
		{
			pthread_cond_broadcast (&c_cons);	
		}
		else
		{
			pthread_cond_signal (&c_prod);
		}
	pthread_mutex_unlock(&m);
	return 0;
}








