#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <arpa/inet.h> /* inet_ntoa() */
#include <stdio.h> /* scanf(), perror() */
#include <stdlib.h> /* exit() */
#include <string.h> /* memset() */
#include <unistd.h>/* getpid */
#include "chatDefs.h"

/* HELPER */
static int SetSockReusable(int _sock);
static int GetSock();
static struct sockaddr_in InitComm(int _sock, int _port, char* _groupAddr);
static int InitMulticast(int _sock, char* _addr);

int ListenerRun(int _socket, struct sockaddr_in _addr, char* _groupName);
static void savePid(char *_grpName);
/* end HELPER */

/* MAIN */

int main(int argc, char *argv[])
{
    int sock, port;
    char* groupAddr;
    char* groupName;
    struct sockaddr_in addr;
    /*pid*/
    savePid(argv[3]);
    /*endpid*/
    if (argc != 4) 
    {
       perror("Args fail");
       exit(1);
    }

    groupAddr = argv[1];
    sscanf(argv[2], "%d", &port);   
    groupName = argv[3];

    sock = GetSock();
    SetSockReusable(sock);
    addr = InitComm(sock, port, groupAddr);
    InitMulticast(sock, groupAddr);

    ListenerRun(sock, addr, groupName);

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

int ListenerRun(int _socket, struct sockaddr_in _addr, char* _groupName)
{
    char buffer[BUFFER_SIZE];
    int readBytes;
    int addrlen = sizeof(_addr);

    printf("Welcome to group: %s!\n", _groupName);
    printf("Please wait to receive messages:\n\n\n");

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

static void savePid(char *_grpName)
{
    FILE* savePIDTo;
    char cPID[10];
    pid_t myPID;

    myPID = getpid();
    savePIDTo = fopen(LISTENER_PID_FNAME, "a");
    
    fprintf(savePIDTo, "%s %d\n",_grpName, myPID);
    fclose(savePIDTo);

}