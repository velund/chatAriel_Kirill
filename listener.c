
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <arpa/inet.h> /* inet_ntoa() */
#include <stdio.h> /* scanf(), perror() */
#include <stdlib.h> /* exit() */
#include <string.h> /* memset() */

#define BUFFER_SIZE 100

/* HELPER */
static int SetSockReusable(int _sock);
static int GetSock();
static struct sockaddr_in InitComm(int _sock, int _port, char* _groupAddr);
static int InitMulticast(int _sock, char* _addr);
int ListenerRun(int _socket, struct sockaddr_in _addr);


/* MAIN */

int main(int argc, char *argv[])
{
    int sock, port;
    char* groupAddr;
    struct sockaddr_in addr;

    if (argc != 3) 
    {
       perror("Args fail");
       exit(1);
    }

    groupAddr = argv[1];
    sscanf(argv[2], "%d", &port);   

    sock = GetSock();
    SetSockReusable(sock);
    addr = InitComm(sock, port, groupAddr);
    InitMulticast(sock, groupAddr);

    ListenerRun(sock, addr);

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



static int SetSockReusable(int _sock)
{
    int yes = 1;
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char*) &yes, sizeof(yes)) < 0)
    {
       perror("Sock reusable failed");
       return 1;
    }
}

static struct sockaddr_in InitComm(int _sock, int _port, char* _groupAddr)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_groupAddr); 
    addr.sin_port = htons(_port);

    if (bind(_sock, (struct sockaddr*) &addr, sizeof(addr)) < 0) 
    {
        perror("bind");
        exit (1);
    }

    return addr;
}

static int InitMulticast(int _sock, char* _addr)
{
    struct ip_mreq mreq;

    mreq.imr_multiaddr.s_addr = inet_addr(_addr);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if ( setsockopt( _sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        exit (1);
    }
}

int ListenerRun(int _socket, struct sockaddr_in _addr)
{
    char buffer[BUFFER_SIZE];
    int readBytes;
    int addrlen = sizeof(_addr);

    while (1) 
    {
        readBytes = recvfrom(_socket ,buffer, BUFFER_SIZE, 0, (struct sockaddr *) &_addr, &addrlen);
        if (readBytes < 0) 
        {
            perror("recvfrom");
            return 1;
        }
        buffer[readBytes] = '\0';
        printf("%s", buffer);
    }
}