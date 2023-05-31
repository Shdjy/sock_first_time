#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

#define SIZE 1024
int main(int argc, char* argv[])
{
	char* ip = argv[1];
	int port = atoi(argv[2]);
	
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port = htons(port);
	int sock = socket(AF_INET,SOCK_STREAM,0);
	assert(sock >= 0);

	int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
	assert(ret != -1);
	ret = listen(sock, 5);
	assert(ret != -1);
	struct sockaddr_in client;
	socklen_t client_addrlenght = sizeof(client);

	int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlenght);
	char buffer_recv[SIZE] = {0};
	char buffer_send[SIZE] = {0};
	while(1)
	{
		if(connfd < 0)
		{
			printf("errno is : %d\n", errno);
		}
		memset(&buffer_recv, 0, SIZE);
		memset(&buffer_send, 0, SIZE);
		recv(connfd, buffer_recv,SIZE-1, 0);
		 if(strcmp(buffer_recv,"over\n") == 0){
                printf("Communications is over!\n");
                break;
            }
            printf("client:%s", buffer_recv);

            printf("server:");
            fgets(buffer_send,SIZE,stdin);
            send(connfd,buffer_send,strlen(buffer_send),0);

		if(strcmp(buffer_send, "over\n") == 0)
		{
			printf("通信结束\n");
			break;
		}
	}
	close(connfd);
	close(sock);
	return 0;
}



