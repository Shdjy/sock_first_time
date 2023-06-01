#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<assert.h>

#define SIZE 1024

void* thread_recv(void* arg)
{
	int connfd = *(int*)arg;
	char buffer_recv[SIZE] = {0};
	while(1)
	{
		memset(buffer_recv, 0, SIZE);
		int ret = recv(connfd, buffer_recv,SIZE-1,0);
		if(ret <= 0)
		{
			printf("对方强制退出 \n");
			break;
		}
		else if(strcmp(buffer_recv,"over\n") == 0)
			{
				printf("通信结束\n");
				break;
			}
			printf("client:%s",buffer_recv);
		
	}
}

void* thread_send(void* arg)
{
	int connfd = *(int*)arg;
	char buffer_send[SIZE] = {0};
	while(1)
	{
		memset(buffer_send, 0, SIZE);
		fgets(buffer_send,SIZE,stdin);
		send(connfd, buffer_send,strlen(buffer_send), 0);
		if(strcmp(buffer_send, "over\n") == 0)
		{
			printf("通信结束\n");
			break;
		}
		
	}
}

int main(int argc, char* argv[])
{
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	int sock = socket(AF_INET, SOCK_STREAM,0);
	assert (sock >= 0);
	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert (ret !=-1);
	ret = listen(sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlenght = sizeof(client);
	int connfd = accept(sock, (struct sockaddr*)&client,&client_addrlenght);
	
	
		if(connfd < 0)
		{
			printf("errno is: %d\n",errno);
			
		}

		pthread_t th1, th2;
		ret = pthread_create(&th1, NULL, thread_recv,&connfd);
		if(ret != 0)
		{
			printf("创建失败");
		}
		ret = pthread_create(&th2, NULL, thread_send,&connfd);
		if(ret != 0)
		{
			printf("failed");
			
		}
		pthread_join(th1,NULL);
		pthread_join(th2,NULL);
	close(connfd);
	close(sock);
	return 0;
}

	
