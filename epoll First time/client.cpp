#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr("192.168.93.129");
    serAddr.sin_port = htons(9999);

    //bind(sockfd, (sockaddr*)&serAddr, sizeof(serAddr));

    connect(sockfd,(sockaddr*)&serAddr, sizeof(serAddr));

    char buf[1024];
    
    while(1)
    {
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t writeBytes = write(sockfd, buf, sizeof(buf));
        if (writeBytes == -1)
        {
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t readBytes = read(sockfd, buf, sizeof(buf));
        if(readBytes > 0)
        {
            printf("server: %s\n", buf);
        }
        
    }
    close(sockfd);
    return 0;
    
}
