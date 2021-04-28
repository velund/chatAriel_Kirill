#include <sys/socket.h> /* socket , bind*/
#include <stdlib.h> /* exit */
#include <errno.h> /* perror */
#include <stdio.h>
#include <string.h> /* memset */
#include <netinet/in.h> /*  struct sockaddr_in*/
#include <unistd.h> /* close */
#include <arpa/inet.h> /* inet_addr */


#define MAXLINE 200
#define PING_PONGS 10

#define GROUP_IP "224.0.0.1"
#define PORT 8080

int main(void)
{
    int sock;
    char buffer[MAXLINE];
    struct sockaddr_in server;
    char *message = "Hello from client";
    int sent = 0, received = 0, serLen, cnt = 0;

    int pongRec[PING_PONGS] = {0};

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) /*can be -1 */
    {
        perror("Socket not initalized! \n");
        exit(1);
    } 
    printf("Client socket : %d \n", sock);

    memset(&server, 0, sizeof(server)); /* memset - zero down all variables in struct */
    
    server.sin_family = AF_INET; /* Set to IPV4 */
    server.sin_addr.s_addr = inet_addr(GROUP_IP);
    server.sin_port = htons(PORT); /* host to network short - 16 bits, htonl - host to network long, 
    makes general host number, can be in 1024 - 64k*/ 

    sleep(2);


    sent = sendto(sock, (const char *)message, strlen(message), 0, (struct sockaddr*) &server, sizeof(server));
    if(sent < 0)
    {
        perror("send failed! \n");
        exit(3);
    }
    printf("Message from client sent \n");

    received = recvfrom(sock, buffer, MAXLINE, 0, (struct sockaddr *) &server, &serLen);
    buffer[received] = '\0';

    printf("Server Message: %s \n", buffer);
    
    while(cnt < PING_PONGS)
    {
        sent = sendto(sock, (void*)&cnt, sizeof(int), 0, (struct sockaddr*) &server, sizeof(server));
        printf("Ping %d from client sent\n", cnt);
       

        received = recvfrom(sock, &pongRec[cnt], sizeof(int), 0, (struct sockaddr *) &server, &serLen);
        printf("Server Pong: %d \n", pongRec[cnt]);
        cnt++;
    }


    close(sock);
}
