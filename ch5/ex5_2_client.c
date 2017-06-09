#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

void str_cli(FILE *fp,int sockfd)
{

}
int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		printf("usage: tcpcli<IPaddress>\n");
		exit(-1);
	}
	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9877);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);

	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	str_cli(stdin,sockfd);
	exit(0);
}
