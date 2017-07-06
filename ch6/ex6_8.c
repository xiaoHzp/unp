#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "unp.h"

int main()
{
	int i,maxi,maxfd,listenfd,connfd,sockfd;
	int nready,client[FD_SETSIZE];
	ssize_t n;
	char buf[MAXLINE];
	fd_set rset,allset;
	socklen_t clilen; 
	struct sockaddr_in cliaddr,servaddr;

	listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	listen(listenfd,5);

	maxfd = listenfd;
	maxi = -1;
	for(i = 0;i<FD_SETSIZE;i++)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);

	while(1)
	{
		rset = allset; // 如果不这么做，那每次都要重新赋值allset
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);

		if(FD_ISSET(listenfd,&rset))
		{
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd,(SA*)&cliaddr,&clilen);

			for(i = 0;i < FD_SETSIZE;i++)
				if(client[i] == -1){
					client[i] = connfd;
					break;
				}
			if(i == FD_SETSIZE)
			{
				printf("too many clients\n");
				exit(-1);
			}
			FD_SET(connfd,&allset);

			if(connfd > maxfd)
				maxfd = connfd;
			if(i > maxi)
				maxi = i;
			if(--nready <= 0)
				continue;
		}

		for(i = 0;i < maxi;i++)
		{
			if((sockfd = client[i]) == -1)
				continue;
			if(FD_ISSET(sockfd,&rset))
			{
				if((n = readn(sockfd,buf,MAXLINE)) == 0)
				{
					close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i] = -1;
				}else
					writen(sockfd,buf,n);
				if(--nready <= 0)
					break;
			}
		}
	}
}
