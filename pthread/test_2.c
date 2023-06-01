#include<stdio.h>
#include<pthread.h>

void* create(void* arg)
{
	int i = *(int*)arg;
	printf("i am %d\n", i);
	return NULL;
}
int main()
{
	int i = 3;
	pthread_t t1;
	pthread_create (&t1,NULL, create,(void*)&i);
	pthread_join(t1,NULL);
	return 0;
}
