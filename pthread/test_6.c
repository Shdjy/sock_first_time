#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem;

void* createf1(void* arg)
{
	printf("t1 wait condition\n");
	sem_wait(&sem);
	printf("t1 go\n");
	return NULL;
}

void* createf2(void* arg)
{
	printf("t2 signal condition\n");
	sem_post(&sem);
	return 0;
}

int main()
{
	sem_init (&sem,0,0);
	pthread_t t1, t2;
	pthread_create(&t1,NULL, createf1, NULL);
	pthread_create(&t2,NULL, createf2, NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
