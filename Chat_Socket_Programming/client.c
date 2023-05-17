#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
//argv[1] server ip address  argv[2] portno

int main (int argc , char **argv)
{
    int sockfd , portno , n;
    char buff[255];
    struct sockaddr_in server1;
    struct hostent *server;
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd<0)
    {
        perror("error open socket");
    }

    server = gethostbyname(argv[1]);
    if(server==NULL)
    {
        fprintf(stderr,"Error,no such host");
    }
    bzero((char *)&server1,sizeof(server1));
    server1.sin_family= AF_INET;
    bcopy((char*)server->h_addr,(char *)&server1.sin_addr.s_addr,server->h_length);

    server1.sin_port = htons(portno);

    if(connect(sockfd,(struct sockaddr *)&server1,sizeof(server1))<0)
    {
        perror("Connect error");
    }

    while(1)
    {
        bzero(buff,255);
        fgets(buff,255,stdin);
        n = write(sockfd,buff,strlen(buff));
        if(n<0)
        {
            perror("Not write");
        }
        bzero(buff,255);
        n = read(sockfd,buff,255);
        if(n<0)
        {
            perror("Not read");
        }
        printf("Server: %s",buff);
        int i = strncmp("Bye",buff,3);
        if(i==0)
            break;
    }

    close(sockfd);
    return 0;
}