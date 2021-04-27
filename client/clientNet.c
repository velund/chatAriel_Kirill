#include <sys/socket.h> /* socket() */
#include <errno.h> /* perror() */
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h> /* memset() printf() */
#include <stdlib.h> /* abort() */
#include <unistd.h> /* close() */
#include <strings.h> /* memset */
#include <string.h> /* memset */
#include <arpa/inet.h> /* inet_addr() */

#include "clientNet.h"
#include "list/DoubleLinkedListGeneric/DoubleLL/ADTErr.h"
#include "list/DoubleLinkedListGeneric/DoubleLL/DoubleLL.h"
#include "list/DoubleLinkedListGeneric/DoubleLLItr/DoubleLLItr.h"
#include "list/DoubleLinkedListGeneric/DoubleLLItr/DoubleLLItr2.h"

struct Client
{
	int m_clientSocket;
	char *m_buffer; /*  */
	List *m_connectedGroups;
};

struct Group
{
	struct sockaddr_in m_groupAddr;
	char *m_grpName;
};

/* assist funcs */
void initServerAddr(struct sockaddr_in *_serverAddr);
/* end assist funcs */

Client *connectClient()
{
	int connection;
	struct sockaddr_in serverAddr;
	Client *client = (Client*)malloc(sizeof(Client));
	if (client == NULL) { return NULL; }
	client->m_buffer = NULL;

	client->m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->m_clientSocket < 0) { free(client); return NULL; }
	
	initServerAddr(&serverAddr);
	
	connection = connect(client->m_clientSocket, (struct sockaddr*)(&serverAddr), sizeof(serverAddr));
	if (connection < 0) {free(client);  return NULL; }
	return client;
}

void disconnect(Client **_client)
{
	close((*_client)->m_clientSocket);
	free(*_client);
	*_client = NULL;
}

clientNetErr sendMsg(int _client_socket, char *_messageFromClient, size_t _msgLen)
{
	int sent_bytes=0;
	sent_bytes = send(_client_socket, _messageFromClient, _msgLen, 0);
	printf("send bytes = %d \n", sent_bytes);
	if (sent_bytes < 0){ return SEND_FAIL; }
	return OK;
}

clientNetErr recvMsg(int _client_socket, int _maxMsgSize, char *_msgFromServer)
{
	int read_bytes;
	read_bytes = recv(_client_socket, _msgFromServer, _maxMsgSize, 0);
	/* 
		while check != full do recv
		PROTOCOL_ERR ProtocolCheck(PackedMessage _packMsg, size_t _rcvMsgSize); 
		PROTOCOL_MSG_FULL,
    	PROTOCOL_MSG_PART
		tmp_buffer = malloc(size = read_bytes)
		memcpy(client_buffer+end of last part, tmp) agregate information
	*/
	if (read_bytes < 0) { return RECV_FAIL; }
	return OK;
}


/* assist funcs */
void initServerAddr(struct sockaddr_in *_serverAddr)
{
	memset(_serverAddr, 0, sizeof(*_serverAddr));
	_serverAddr->sin_family = AF_INET;
	_serverAddr->sin_addr.s_addr = inet_addr(SERVER_IP);
	_serverAddr->sin_port = htons(SERVER_PORT);
}
/* getters */
int getClientSocket(Client *_client)
{
	if (_client == NULL) { return -1; }
	return _client->m_clientSocket;
}
