#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <time.h>
	#include <sys/types.h>
	#include <netdb.h>
	#include <netinet/ip.h>
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>

	#include <signal.h>
	#include <stdio.h>
	#include <string.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <sys/time.h>
	#include <stdlib.h>
	#include <memory.h>
	#include <ifaddrs.h>
	#include <net/if.h>
	#include <stdarg.h>
	#include <netinet/in.h>
	#include <unistd.h>

#define MSGBUFSIZE 256


int main(int argc, char const *argv[])
{
	char groupAddr[30];
	int groupPort;
	struct sockaddr_in addr;
	unsigned int addrLen = sizeof(addr);
	struct ip_mreq mreq;
	int yes = 1;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(groupAddr, argv[1]);
	sleep(10);
	sscanf(argv[2], "%d", &groupPort);
	printf("ip: %s, port: %d\n",groupAddr , groupPort);
	if (fd < 0) 
	{
		perror("socket");
		return 1;
	}
	/*allow multiple sockets to use the same PORT number*/
	
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(yes)) < 0)
	{
	   perror("Reusing ADDR failed");
	   return 1;
	}

		/* set up destination address*/
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(groupAddr); 
	addr.sin_port = htons(groupPort);

	/*bind to receive address*/
	
	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	} 

	/* use setsockopt() to request that the kernel join a multicast group*/
	
	mreq.imr_multiaddr.s_addr = inet_addr(groupAddr);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,  &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt\n");
		return 1;
	}

	while (1) 
	{
		char msgbuf[MSGBUFSIZE];
		int addrlen = sizeof(addr);
		int nbytes = recvfrom(fd,msgbuf,MSGBUFSIZE,0,(struct sockaddr *) &addr,&addrLen);
		if (nbytes < 0) 
		{
			perror("recvfrom");
			return 1;
		}
		msgbuf[nbytes] = '\0';
		puts(msgbuf);
		
		fflush(stdin);
	 }

	return 0;
}