#include<stdio.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

//pthread_mute_t mutex = PTHREAD_MUTEX_INTITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INTITIALIZER;

int flag = 0;

void* createf1(void* arg)
{
	printf("t1 wait condition\n" );
	pthread_mutex_lock(&mutex);
	while(flag ==0 )
	{
		pthread_cond_wait(&cond,&mutex);
	}
	printf("t1 go\n");
	pthread_mutex_unlock;
	return NULL;
}

void* createf2(void* arg)
{
	printf("t2 signal condition\n");
	pthread_mutex_lock(&mutex);
	flag = 1;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_t t1,t2;
	pthread_create(&t1,NULL, createf1, NULL);
	pthread_create(&t2,NULL, createf2, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
