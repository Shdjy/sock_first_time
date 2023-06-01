#include<stdio.h>
#include<pthread.h>

void* create (void* arg)
{
	int i = 0;
	for(i = 0;i <= 2;i++)
	{
		printf("i am createf\n");
	}
	return NULL;
}
int main()
{
	int i = 0;
	pthread_t t1;
	int ret = pthread_create(&t1, NULL,create, NULL);
	if(ret != 0)
	{
		printf("创建失败\n");
	}
	for(i = 0;i <= 5;i++)
	{
		printf("i am mainf\n");
	}
	pthread_join (t1,NULL);
	return 0;
}
