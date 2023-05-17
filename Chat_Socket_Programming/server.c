#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

int main (int argc , char **argv)
{
    int sockfd , newsockfd , portno , n;
    char buff[255];
    struct sockaddr_in server , client;
    socklen_t clen;

    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("error open socket");
    }

    bzero((char *)&server,sizeof(server));
    portno = atoi(argv[1]);

    //unlink(portno);

    server.sin_family= AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port = htons(portno);

    if(bind(sockfd,(struct sockaddr *)&server,sizeof(server))<0)
    {
        perror("Bind error");
    }

    listen(sockfd,5);
    clen=sizeof(client);
    newsockfd = accept(sockfd,NULL,NULL);
    if(newsockfd<0)
    {
        perror("Accept error");
    }

    while(1)
    {
        bzero(buff,255);
        n = read(newsockfd,buff,255);
        if(n<0)
        {
            perror("Not read");
        }
        printf("Client : %s\n",buff);
        bzero(buff,255);
        fgets(buff,255,stdin);

        n = write(newsockfd,buff,strlen(buff));
        if(n<0)
        {
            perror("Not write");
        }
        int i = strncmp("Bye",buff,3);
        if(i==0)
            break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;



}