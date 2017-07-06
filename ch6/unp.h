#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define MAXLINE 4096
#define SERV_PORT 9877
typedef struct sockaddr SA;
typedef void (*Sigfunc)(int);

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d terminated\n",pid);

	return;
}
Sigfunc Signal(int signo,Sigfunc func)
{
	struct sigaction act,oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM)
	{
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	}
	else
	{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if(sigaction(signo,&act,&oact) < 0)
		return SIG_ERR;
	return oact.sa_handler;
}
ssize_t readn(int fd, void *vptr, size_t n) // read n bytes from a descriptor
{
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nread = read(fd,ptr,nleft)) < 0){
			if(errno == EINTR)
				nread = 0;
			else
				return -1;
		}else if(nread == 0)
			break;
		nleft -= nread;
		ptr +=nread;
	}
	return (n-nleft);
}

ssize_t writen(int fd, const void *vptr, size_t n) // write n bytes to a descriptor
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nwritten = write(fd,ptr,nleft)) <=0 )
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n,rc;
	char c,*ptr;

	ptr = vptr;
	for(n = 1;n < maxlen;n++)
	{
		again:
		if((rc = read(fd,&c,1)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
				break;
		}else if(rc == 0)
		{
			*ptr = 0;
			return n - 1;
		}else
		{
			if(errno == EINTR)
				goto again;
			return -1;
		}
	}
	*ptr = 0;
	return n;
}
