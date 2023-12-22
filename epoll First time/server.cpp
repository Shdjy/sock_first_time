#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 1024


void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void errif(bool condition, const char *errmsg)
{
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}

/// @brief 
/// @return 
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("192.168.93.129");
    servAddr.sin_port = htons(9999);

    bind(sockfd, (sockaddr*)&servAddr,sizeof(servAddr));

    listen(sockfd, 5);

    int epfd = epoll_create1(0);
    if (epfd == -1)
    {
        printf("epoll create error\n");
    }

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events,sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;
    setnonblocking(sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);

    while (1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            printf("epoll wait error");
        }

        for(int i=0; i<nfds; ++i)
        {
            if(events[i].data.fd == sockfd)
            {
                struct sockaddr_in clientAddr;
                socklen_t clientLen = sizeof(clientAddr);
                bzero(&clientAddr,sizeof(clientAddr));
                int clientSockfd = accept(sockfd, (sockaddr*)&clientAddr, &clientLen);
                errif(clientSockfd == -1, "socket accept error");

                printf("new client fd %d! ip %s, port %d\n", clientSockfd, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = clientSockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clientSockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientSockfd, &ev);

            }
            else if (events[i].events & EPOLLIN)
            {
                char buf[1024];
                while (1)
                {
                    bzero(&buf, sizeof(buf));
                    ssize_t readBytes = read(events[i].data.fd, buf, sizeof(buf));
                    if(readBytes > 0)
                    {
                        printf("message from client fd %d:%s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, sizeof(buf));
                    }
                    else if (readBytes == -1 && errno == EINTR)
                    {
                        printf("continue reading");
                        continue;
                    }
                    else if (readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                    {
                        printf("finish reading once.");
                        break;
                    }
                    else if(readBytes == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                    
                    
                }
                
            }
            else
            {
                printf("other!");
            }
            
        }
    }
    
    close(sockfd);
  
    return 0;

}
