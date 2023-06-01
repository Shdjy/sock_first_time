#include<stdio.h>
#include<pthread.h>

struct INFO{
	int age;
	char*  name;
}shd;

void* create(void* agc)
{
	struct INFO shd = *(struct INFO*)agc;
	printf("i am %s,age:%d\n",shd.name, shd.age);
	return NULL;
}
int main()
{
	shd.age = 23;
	shd.name = "sunhaodng";
	pthread_t t1;
	pthread_create(&t1, NULL, create, (void*)&shd);
	pthread_join(t1,NULL);
	return 0;
}

