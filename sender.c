#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <arpa/inet.h> /* inet_ntoa() */
#include <stdio.h> /* scanf(), perror() */
#include <stdlib.h> /* exit() */
#include <string.h> /* memset() */

#define BUFFER_SIZE 100

/* HELPER */
static int GetSock();
static struct sockaddr_in InitComm(int _sock, int _port, char* _group);
void SenderRun(int _socket, struct sockaddr_in _addr, char* _userName);


/* MAIN */

int main(int argc, char *argv[])
{
    int sock, port;
    char* group, *userName;
    struct sockaddr_in addr;

    if (argc != 4) 
    {
       perror("Args fail");
       exit(1);
    }

    group = argv[1];
    sscanf(argv[2], "%d", &port);  
    userName = argv[3];

    sock = GetSock();
    addr = InitComm(sock, port, group);

   SenderRun(sock, addr, userName);
    

    return 0;
}


/* ------------------------ HELPER ------------------------ */ 


static int GetSock()
{
    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) 
    {
        perror("socket get err");
        exit(1);
    }
}

static struct sockaddr_in InitComm(int _sock, int _port, char* _group)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_group);
    addr.sin_port = htons(_port);

    return addr;
}



void SenderRun(int _socket, struct sockaddr_in _addr, char* _userName)
{
    int sentBytes;
    char buffer[BUFFER_SIZE];
    char finalMsg[BUFFER_SIZE];

    while (1) 
    {
        sprintf(finalMsg, "%s: ", _userName);
        fgets(buffer, BUFFER_SIZE, stdin);
        strcat(finalMsg, buffer);
        
        sentBytes = sendto(_socket, finalMsg, strlen(finalMsg), 0, (struct sockaddr*) &_addr, sizeof(_addr) );
            
        if (sentBytes < 0) 
        {
            perror("sendto");
            exit(1);
        }
    }
}