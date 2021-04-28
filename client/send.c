#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h> 

	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
    #define MAX_MESSAGE_SIZE 200
int main(int argc, char *argv[])
{
	const char *message[200];
	struct sockaddr_in sendingAddr;
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) 
    {
		perror("socket creation fail\n");
		return 1;
	}
	if (argc != 3) 
	{
		perror("num of args not correct\n");
		return 1;
	}

    char* group = argv[1]; // e.g. 239.255.255.250 for SSDP
    int port = atoi(argv[2]); // 0 if error, which is an invalid port

	memset(&sendingAddr, 0, sizeof(sendingAddr));
	sendingAddr.sin_family = AF_INET;
	sendingAddr.sin_addr.s_addr = inet_addr("224.0.0.1");
	sendingAddr.sin_port = htons(1026);

	while (1) 
    {
        fgets(message, MAX_MESSAGE_SIZE, stdin);
		int nbytes = sendto(fd,message,strlen(message),0,(struct sockaddr*) &sendingAddr,sizeof(sendingAddr));
		if (nbytes < 0) 
		{
			perror("send fail\n");
			return 1;
		}
		sleep(1); 

	 }

	return 0;
}