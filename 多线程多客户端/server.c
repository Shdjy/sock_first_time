#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define SIZE 1024
int i = 0;

void* accept_thread(void* agc)
{
	int connfd = *(int*)agc;
	
	free(agc);
	char buffer_recv[SIZE] = {0};
	char buffer_send[SIZE] = {0};

	while (1)
	{
		memset(buffer_recv, 0, SIZE);
		memset(buffer_send, 0, SIZE);
		int ret = recv(connfd, buffer_recv,SIZE-1, 0);
		if(ret <=0)
		{
			printf("error\n");
			break;
		}
		else if(strcmp(buffer_recv, "q\n") == 0)
		{
			printf("over\n");
			break;
		}
		printf("client%d:%s",i, buffer_recv);

		printf("server:");
		fgets(buffer_send, SIZE,stdin);
		send(connfd, buffer_send,strlen(buffer_send),0);

		pthread_mutex_unlock(&mutex);
		if(strcmp(buffer_send, "q\n") == 0)
		{
			printf("over\n");
			break;
		}
	}

	//pthread_mutex_unlock(&mutex);
	close(connfd);
	return NULL;
}
int main(int argc, char* argv[])
{
	char* ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip, &address.sin_addr);
	address.sin_port = htons(port);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);
	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert(ret !=-1);

	ret = listen (sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	
	while(1)
	{
		socklen_t client_len = sizeof(client);

		pthread_mutex_lock(&mutex);
		int connfd = accept(sock, (struct sockaddr*)&client,&client_len);
		i++;
		if(connfd < 0)
		{
			printf("errno is %d\n", errno);
			break;
		}

		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,&client.sin_addr, client_ip,INET_ADDRSTRLEN);
		printf("client ip is:%s\n",client_ip);

		pthread_t thread;
		int* pconnfd = malloc(sizeof(int));
		*pconnfd = connfd;
	
		pthread_create(&thread, NULL, accept_thread,(void* )pconnfd);
		pthread_detach(thread);
	}
	close(sock);
	return 0;
}



