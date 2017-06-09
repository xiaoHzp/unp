#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct sockaddr SA;
int main(int argc,char* argv[])
{
	int listenfd,connfd;
	struct sockaddr_in servaddr;
	char buff[4096];
	time_t ticks;

	listenfd = socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(4000);

	if(bind(listenfd,(SA*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("bind error\n");
		exit(1);
	}
	listen(listenfd,5);
	connfd = accept(listenfd,(SA*)NULL,NULL);
	ticks = time(NULL);
	snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
	write(connfd,buff,strlen(buff));
	close(connfd);
}
