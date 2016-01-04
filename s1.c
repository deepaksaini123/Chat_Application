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
#include    <pthread.h>

#define	MAXLINE	100
#define	SA	struct sockaddr
#define	LISTENQ		1024
int connfd[20];
void* str_echo(void *arg)
{
	int *temp=(int *)arg;//extracting sockfd of client
	int sockfd=*temp;
	
	int sockfd_temp,write_fd;
	char sock_fds[5];int i;
	ssize_t n;
	char message_read[MAXLINE],message_write[MAXLINE],fin_message[MAXLINE],copy[MAXLINE];
	again:
		while(1)
		{
			for(i=0;i<MAXLINE;i++)
			{
				message_read[i]='\0';
				message_write[i]='\0';
				fin_message[i]='\0';
				//copy[i]='\0';
			}
			n=read(sockfd, message_read, MAXLINE);
			if(n > 0)
			{
				// code for logout
				if(strcmp(message_read,"logout")==0)
				{
					i=0;
					while(connfd[i]!=-1)
					{
		               if(connfd[i]==sockfd)
					   {    
						   connfd[i]=-1;
						   break;
					   }
					   i++;
					}
					close(sockfd);
					return NULL;
				}
		        
				
				for(i=0;i<MAXLINE;i++)
				    copy[i]=message_read[i];
				printf("\nputs  = ");puts(copy);
				sscanf(message_read,"%d %d %s",&sockfd_temp,&write_fd,message_write);//it extracts message upto first space only
			    //puts(message_write);
         	    sprintf(sock_fds,"%d",sockfd);//changing int sock_fd to string sockfds
         	    //preparing message for client by concatenating
			    puts(sock_fds);
			    strcat(fin_message,sock_fds);
			    strcat(fin_message," : ");
			    //temp code
			    for(i=0;i<4;i++)
			        copy[i]=' ';
			    puts(copy);
			    strcat(fin_message,copy);
			    puts(fin_message);
		        write(write_fd,fin_message,n+10);
			}
			else if(n<0 &&errno == EINTR)
		        goto again;
		    else if(n < 0)
		        printf("at the server : read error");
	    }
}
int check_idp(int);
int main(int argc ,char **argv)
{
	int serv_port,i;
	for(i=0;i<20;i++)
	    connfd[i]=-1;
	sscanf(argv[2],"%d",&serv_port);
    int listenfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);//creating socket
	//printf("creating socket at the server and listenfd = %d\n",listenfd);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serv_port);
    
    int bind_id=bind(listenfd, (SA *) &servaddr, sizeof(servaddr));//binding the socket
    //printf("socket binded and bind id= %d\n",bind_id);
    
    int lid=listen(listenfd, LISTENQ);//listening to client
    //printf("listenoing to clients and id= %d\n\n\n",lid);
    i=0;
    pthread_t tid[5];char idp[MAXLINE];
    for(;;)
    {
    	clilen = sizeof(cliaddr);char loc[MAXLINE];
    	
    	connfd[i]= accept(listenfd, (SA *) &cliaddr, &clilen);//accepting the connection with the client
    	printf("connection established with a new client , connection id = %d\n",connfd[i]);
    	
    	//verifying id and password
    	//printf("\nreading idp");
    	ssize_t n=read(connfd[i], idp, MAXLINE);
    	//printf("\n idp from client is \n");
    	puts(idp);
    	int x=0;char idp_arr[MAXLINE],ch;
    	FILE *fp=fopen("idp.txt","r");
    	int ii=0,jj,kk;
	    for(jj=0;jj<100;jj++)
	        idp_arr[jj]='\0';
	    while(1)
	    {//printf("inwhile\n");
		    ch=fgetc(fp);
		    if(ch == EOF)
		    {   x=0;break;}
		    else if(ch=='\n')
		    {
			    //function for string matching
			    kk=0;
			    while(idp_arr[kk]!='\0')
			    {
				    if(idp_arr[kk]!=idp[kk])
					    break;
				    kk++;
			    }
			    if(strlen(idp_arr) != kk)
			    {
				    ii=0;
				    for(jj=0;jj<100;jj++)
	                    idp_arr[jj]='\0';
	                continue;
			    }
			    else
			    {
     			    x=1;break;     		
     			}
		    }
		    else //saving id password to array
			    idp_arr[ii++]=ch;
	    }
    	
    	
    	if(x==1) 	
    	{
			write(connfd[i],"\ncorrect id and password\n start chatting ...\n",48);
			//displaying list of clients    	
    	    int k=0;
    	    while(k<20)
    	    {
			    if(connfd[k]!=-1)
			    {
				    write(connfd[k],"online clients  ",29);
			        int j=0;char ch[4];
    	            while(j<20)
    	            {
			            if(connfd[j]!= -1)
			            {
					        ch[2] = '\0';
			                sprintf(ch,"%d",connfd[j]);
			                strcat(ch," ");
			                write(connfd[k],ch,strlen(ch));
				        }
				        j++;
		             }
			     }
			     k++;
		     }
        	 int err;
	         if((err = pthread_create(&tid[i], NULL, &str_echo, &connfd[i])) != 0)
    	         printf("thread not created");
           	i++;
		}
    	else 
    	    write(connfd[i],"\nwrong id and password \n",25);
    	
    }
}
