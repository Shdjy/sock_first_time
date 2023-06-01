#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>

#define SIZE 1024

void* thread_send(void*arg)
{
	int sockfd = *(int*)arg;
	char buffer_send[SIZE] = {0};
	while(1)
	{
		memset (buffer_send,0,SIZE);
		fgets(buffer_send, SIZE, stdin);

		int ret = send(sockfd,buffer_send,strlen(buffer_send),0);
		
		if(strcmp(buffer_send, "over\n") == 0)
		{
			printf("通信结束");
			break;
		}
		
	}
}
void* thread_recv(void* arg)
{
	int sockfd = *(int *)arg;
	char buffer_recv[SIZE] = {0};
	while(1)
	{
		memset(buffer_recv,0,SIZE);

		int ret = recv(sockfd, buffer_recv, SIZE,0);
		if(ret <= 0)
		{
			printf("对方强制退出 \n");
			break;
		}
		else if(strcmp(buffer_recv,"over\n") == 0)
		{
			printf("通信结束");
			break;
		}
		printf("server:%s",buffer_recv);

	}
}

int main(int argc, char* argv[])
{
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in server_address;
	memset(&server_address,0, sizeof(server_address) );
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, ip,&server_address.sin_addr);
	server_address.sin_port = htons(port);
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd >= 0);
	int connfd = connect(sockfd, (struct sockaddr*)&server_address,sizeof(server_address));
	
	
		if(connfd < 0)
		{
			printf("通信失败\n");
			
		}
		pthread_t th1, th2;
		int ret = pthread_create(&th1, NULL,thread_send,&sockfd);
		if(ret != 0)
		{
			printf("创建失败");
			
		}
		ret = pthread_create(&th2,NULL,thread_recv,&sockfd);
		if(ret != 0)
		{
			printf("创建失败");
			
		}


		pthread_join(th1, NULL);
		pthread_join(th2, NULL);
	
	close(connfd);
	close(sockfd);
	return 0;
}
	

