#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct sockaddr  SA;

int main(int argc,char* argv[])
{
	int sockfd,n;
	struct sockaddr_in sockaddr;
	char recvline[4096];

	if(argc != 2)
	{
		printf("usage:a.out <IPaddress>\n");
		exit(1);
	}
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket error\n");
		exit(1);
	}
	memset(&sockaddr,0,sizeof(sockaddr));
	//	bzero(&sockaddr,sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(3000);
	if(inet_pton(AF_INET,argv[1],&sockaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s\n",argv[1]);
		exit(1);
	}
	if(connect(sockfd,(SA*)&sockaddr,sizeof(sockaddr)) < 0)
	{
		printf("connect error\n");
		exit(1);
	}

	while((n = read(sockfd,recvline,4096))>0)
	{
		recvline[n] = 0;
		if(fputs(recvline,stdout) == EOF)
		{
			printf("fputs error\n");
			exit(1);
		}
	}
	if(n < 0)
	{
		printf("read error\n");
		exit(1);
	}
	exit(0);
}
