#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "unp.c"

#define SERV_PORT 9877
typedef struct sockaddr SA;

void str_echo(int);
int main(int argc,char *argv[])
{
	int listenfd,connfd;
	struct sockaddr_in cliaddr,servaddr;
	socklen_t clilen;
	pid_t chlidpid;

	bzero(&servaddr,sizeof(servaddr));
	listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	listen(listenfd,5);
	while(1)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd,(SA*)&cliaddr,&clilen);

		if((chlidpid = fork()) == 0)
		{
			close(listenfd);
			str_echo(connfd);
			close(connfd);
		}
		close(connfd);
	}
	exit(0);
}

void str_echo(int connfd)
{
	char buf[4096];
	int n;
again:
	while((n = read(connfd,buf,4096)) > 0)
		writen(connfd,buf,n);
	if(n < 0 && errno ==  EINTR)
		goto again;
	else if(n < 0)
	{
		printf("str_echo:read error\n");
		exit(-1);
	}
}
