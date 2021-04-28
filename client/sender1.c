 	#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif
	 #include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h> /* for sleep() */

	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#define MAX_MSG_SIZE 200


int main(int argc, char const *argv[])
{
	char* groupAddr;
	int groupPort;
	char *message;
	struct sockaddr_in addr;
	int nbytes;
	size_t size = MAX_MSG_SIZE;
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	message = (char*)malloc(MAX_MSG_SIZE*sizeof(char));
	groupAddr = argv[1];
	groupPort = atoi(argv[2]);
	/* create what looks like an ordinary UDP socket*/
	if (fd < 0) 
	{
		perror("socket");
		return 1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(groupAddr);
	addr.sin_port = htons(groupPort);

	while (1) 
	{
		getline(&message, &size, stdin); /* fgets */ 
		nbytes = sendto(fd,message,strlen(message),0,(struct sockaddr*) &addr,sizeof(addr));
		if (nbytes < 0) 
		{
			perror("sendto");
			return 1;
		}
		fflush(stdin);
	}

	return 0;
}