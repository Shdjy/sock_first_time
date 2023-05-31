#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#define size 1024
int main(int argc, char* argv[])
{
	const char* ip = argv[1];
	int port = atoi(argv[2]);
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	

	inet_pton(AF_INET, ip, &server_address.sin_addr);
	server_address.sin_port = htons(port);

	int sockfd = socket(AF_INET, SOCK_STREAM,0);

	int connfd = connect(sockfd, (struct sockaddr*)&server_address,sizeof(server_address));
	char buffer_recv[size] = {0};
	char buffer_send[size] = {0};
	while(1)
	{
	if(connfd < 0)
	{
		printf("通信失败");
		break;
	}
	else
	{
		memset(&buffer_recv,0, size);
		memset(&buffer_send,0, size);
		printf("客户端:");
		fgets(buffer_send,size,stdin);
		send(sockfd,buffer_send,strlen(buffer_send),0);
		if(strcmp(buffer_send, "over\n") == 0)
		{
			printf("通信结束\n");
			break;
		}
		int ret =recv(sockfd,buffer_recv,size-1,0);
		if(strcmp(buffer_recv,"over\n") == 0)
		{
			printf("通信结束\n");
			break;
		}
		printf("服务端:%s",buffer_recv);
	}
}
	close(connfd);
	close(sockfd);
	return 0;
}

