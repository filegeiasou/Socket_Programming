#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NAME "/tmp/socket"
int main(int argc, char *argv[])
{
int sock, msgsock, rval;
struct sockaddr_un server; // for unix domain
char buf[1024];
sock = socket(AF_UNIX, SOCK_STREAM, 0);//socket(int domain,int type , int protocol)
if (sock < 0) //if not create socket
{
perror("opening stream socket");
exit(1);
}
server.sun_family = AF_UNIX; //init domain
strcpy(server.sun_path, NAME);//init full path
if (bind(sock, (struct sockaddr *) &server,sizeof(struct sockaddr_un)))
//int sockfd(socket) ,const struct sockaddr * , socklen_t addrlen
{
perror("binding stream socket");
exit(1);
}
printf("Socket has name %s\n", server.sun_path);
listen(sock, 5);//5 process
for (;;)
{
msgsock = accept(sock, NULL, NULL);
if (msgsock == -1)
perror("accept");
else
do
{
bzero(buf, sizeof(buf));// do 0 in buf
if ((rval = read(msgsock, buf, 1024)) < 0)
perror("reading stream message");
else if (rval == 0)
printf("Ending connection\n");
else
printf("-->%s\n", buf);
} while (rval > 0);
close(msgsock);
}
close(sock);
unlink(NAME);//delete socket
}
