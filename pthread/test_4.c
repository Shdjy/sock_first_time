#include<stdio.h>
#include<pthread.h>

void* create(void*arg)
{
	return NULL;
}
int main()
{
	pthread_t t1;
	pthread_attr_t attr;
	int state;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&t1,&attr,create,NULL);
	pthread_attr_getdetachstate(&attr,&state);
	if(state == PTHREAD_CREATE_JOINABLE)
	{
		printf("非分离\n");
		pthread_detach(t1);//分离状态下线程推出会自动回收资源
		printf("改为了分离\n");
	}
	else if(state == PTHREAD_CREATE_DETACHED)
	{
		printf("分离\n");
	}
	pthread_attr_destroy(&attr);
	pthread_join(t1,NULL);
	return 0;
}

