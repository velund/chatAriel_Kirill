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

#define MSGBUFSIZE 256
/*char *str;
strcat(str, argv[1]);
system(str);*/
int main(int argc, char *argv[]) 
{
	char* groupAddr;
	int groupPort;
	struct sockaddr_in addr;
	unsigned int addrLen = sizeof(addr);
	struct ip_mreq mreq;
	int yes = 1;
	char message[MSGBUFSIZE];
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) 
	{
		perror("socket creation failure\n");
		return 1;
	}
	if (argc != 3) 
	{
	   	perror("Command line args should be multicast group and groupPort\n");
		return 1;
	}

	groupAddr = argv[1]; // e.g. 239.255.255.250 for SSDP
	groupPort = atoi(argv[2]); // 0 if error, which is an invalid groupPort
	/*allow multiple sockets to use the same PORT number*/
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(yes)) < 0)
	{
	   perror("Reusing ADDR failed");
	   return 1;
	}

		/* set up destination address*/
	
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("224.0.0.1"); 
	addr.sin_port = htons(1026);

	/*bind to receive address*/
	
	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	} 

	/* use setsockopt() to request that the kernel join a multicast groupAddr*/
	mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,  &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
		return 1;
	}

	while (1) 
	{
		char message[MSGBUFSIZE];
		int addrlen = sizeof(addr);
		int nbytes = recvfrom(fd,message,MSGBUFSIZE,0,(struct sockaddr *) &addr,&addrLen);
		if (nbytes < 0) 
		{
			perror("recvfrom");
			return 1;
		}
		message[nbytes] = '\0';
		puts(message); /* ui function(message) */
		putchar('\n');
	 }

	return 0;
}