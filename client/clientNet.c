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
#define GROUP_NAME_LEN 20
struct Client
{
	int m_clientSocket;
	char *m_buffer; /*  */
	List *m_connectedGroups; /* List groups */
};

struct Group
{
	struct sockaddr_in m_groupAddr;
	char m_grpName[GROUP_NAME_LEN];
};

/* assist funcs */
void initAddr(struct sockaddr_in *_serverAddr, char *_ip, int _port);
/* end assist funcs */




clientNetErr addGroup(Client *_client, char *_grpName, char *_ip, int _port)
{
	Group *group;
	if (_grpName == NULL || _ip == NULL || _port < 1025) { return INCORRECT_ARGS_OF_ADD_GROUP; }
	group = (Group*)malloc(sizeof(Group));
	if (group == NULL) { return GROUP_MALLOC_FAILED; }
	strcpy(group->m_grpName, _grpName);
	initAddr(&(group->m_groupAddr), _ip, _port);
	ListPushTail(_client->m_connectedGroups, (void*)group); /* check pushTail return */
	return CLIENT_NET_OK; 
}

Client *connectClient()
{
	int connection;
	struct sockaddr_in serverAddr;
	Client *client = (Client*)malloc(sizeof(Client));
	if (client == NULL) { return NULL; }
	client->m_buffer = NULL;
	client->m_connectedGroups = ListCreate();
	client->m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->m_clientSocket < 0) { free(client); return NULL; }
	
	initAddr(&serverAddr, SERVER_IP, SERVER_PORT);
	
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
	return CLIENT_NET_OK;
}

clientNetErr recvMsg(int _client_socket, int _maxMsgSize, char *_msgFromServer)
{
	int read_bytes;
	read_bytes = recv(_client_socket, _msgFromServer, _maxMsgSize, 0);
	/* 
		while check != full { do recv }
		PROTOCOL_ERR ProtocolCheck(PackedMessage _packMsg, size_t _rcvMsgSize); 
		PROTOCOL_MSG_FULL,
    	PROTOCOL_MSG_PART
		tmp_buffer = malloc(size = read_bytes)
		memcpy(client_buffer+end of last part, tmp) agregate information
	*/
	if (read_bytes < 0) { return RECV_FAIL; }
	return CLIENT_NET_OK;
}


/* assist funcs */
void initAddr(struct sockaddr_in *_serverAddr, char *_ip, int _port)
{
	memset(_serverAddr, 0, sizeof(*_serverAddr));
	_serverAddr->sin_family = AF_INET;
	_serverAddr->sin_addr.s_addr = inet_addr(_ip);
	_serverAddr->sin_port = htons(_port);
}
/* getters und setters*/
int getClientSocket(Client *_client)
{
	if (_client == NULL) { return -1; }
	return _client->m_clientSocket;
}

void setGroupName(Group *_gr, char *_grName)
{
	_gr->m_grpName = _grName; 
}
void setGroupAddr(Group *_gr, char *_grIp, int _grPort)
{
	initAddr(&(_gr->m_groupAddr), _grIp, _grPort);
}
