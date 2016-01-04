#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
# include	<sys/select.h>
#include    <limits.h>
#include<pthread.h>

#define	MAXLINE	4096

#define	SA	struct sockaddr
#define	LISTENQ		1024

int sockfd;
char send_msg[MAXLINE], recv_msg[MAXLINE];

void * write_msg()
{
	int i;
	while(1)
	{
		for(i=0;i<MAXLINE;i++)
		       send_msg[i]='\0';
		gets(send_msg);
		int wid=write(sockfd,send_msg,strlen(send_msg));
	}

}
void * read_msg()
{
	char buf[MAXLINE];
	int n,i;
	while(1)
	{
		for(i=0;i<MAXLINE;i++)
		       buf[i]='\0';
		n=read(sockfd,buf,MAXLINE);
		puts(buf);
	}
}
void str_cli()
{
	
	int n;
	char idp[MAXLINE],try[MAXLINE];
	//verifying for rid and password
	printf("enter id and spassword\n       ");
	gets(idp);
	printf("message read from keyboard is \n        ");
	puts(idp);
	//printf(" id password  writing to server\n");
	int wid=write(sockfd,idp,strlen(idp));
	//printf(" message written to server and id = %d\n",wid);
	n=read(sockfd,try,MAXLINE);
	printf("\nstatus from server is \n");
	puts(try);
	
		   n=read(sockfd,try,MAXLINE);
		   //printf("\nlist of available ids and id =%d\n",n);
		   puts(try);
    pthread_t tid1,tid2;
	    int err;
	    if((err = pthread_create(&tid1, NULL, &write_msg, NULL)) != 0)
    	    printf("thread 1 not created");
    	if((err = pthread_create(&tid2, NULL, &read_msg, NULL)) != 0)
    	    printf("thread  2 not created");
    	pthread_join(tid1,NULL);
    	pthread_join(tid2,NULL);
}
int main(int argc, char *argv[])
{
	int serv_port;
	sscanf(argv[2],"%d",&serv_port);printf("%d",serv_port);
	struct sockaddr_in servaddr;
	
	//if(argc != 2)
	  //  printf("usage : tcpli <ipaddress>\n");
	sockfd= socket(AF_INET, SOCK_STREAM,0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serv_port);
    inet_pton(AF_INET, "127.0.0.7", &servaddr.sin_addr);

 int bind_id=bind(sockfd, (SA *) &servaddr, sizeof(servaddr));//binding the socket
    printf("socket binded and bind id= %d\n",bind_id);
    
    connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
    str_cli(sockfd);
    
    exit(0);
}
