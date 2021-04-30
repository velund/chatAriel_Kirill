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

#include "list.h"

#define GROUP_NAME_LEN 20
#define CLIENT_NAME_LEN 20

struct Client
{
	char m_name[CLIENT_NAME_LEN];
	int m_clientSocket;
	char *m_buffer; /*  */
	int m_numOfGroups;
	List *m_connectedGroups; /* List groups */
	
};

struct Group
{
	struct sockaddr_in m_groupAddr;
	char m_grpName[GROUP_NAME_LEN];
	int m_chatId;
};

/* assist funcs */
void initAddr(struct sockaddr_in *_serverAddr, char *_ip, int _port);
int destroyGroup(void *_element,void *_context);
/* end assist funcs */

clientNetErr addGroup(Client *_client, char *_grpName, char *_ip, int _port, Group **_group)
{
	Group *group;
	if (_grpName == NULL || _ip == NULL || _port < 1025) { return INCORRECT_ARGS_OF_ADD_GROUP; }
	group = malloc(sizeof(Group));
	if (group == NULL) { return GROUP_MALLOC_FAILED; }
	strcpy(group->m_grpName, _grpName);
	_client->m_numOfGroups++;
	group->m_chatId = _client->m_numOfGroups;
	initAddr(&(group->m_groupAddr), _ip, _port);
	if ( (ListPushTail(_client->m_connectedGroups, (void*)group)) != LIST_SUCCESS) { return GROUP_NOT_CREATED ;}
	*_group = group;
	return CLIENT_NET_OK; 
}


Client *connectClient()
{
	int connection;
	struct sockaddr_in serverAddr;
	Client *client = (Client*)malloc(sizeof(Client));
	if (client == NULL) { return NULL; }
	client->m_buffer = NULL;
	client->m_numOfGroups = 0;
	client->m_connectedGroups = ListCreate();
	if(client->m_connectedGroups == NULL)
	{
		free(client);
		return NULL;
	}
	client->m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->m_clientSocket < 0) { free(client); return NULL; }
	
	initAddr(&serverAddr, SERVER_IP, SERVER_PORT);
	
	connection = connect(client->m_clientSocket, (struct sockaddr*)(&serverAddr), sizeof(serverAddr));
	if (connection < 0) {free(client);  return NULL; }
	return client;
}

void disconnect(Client **_client)
{
	close((*_client)->m_clientSocket); /* destroy _client-> */
	free(*_client);
	*_client = NULL;
}

clientNetErr sendMsg(int _client_socket, char *_messageFromClient, size_t _msgLen)
{
	int sent_bytes=0;
	sent_bytes = send(_client_socket, _messageFromClient, _msgLen, 0);
	if (sent_bytes < 0){ return SEND_FAIL; }
	return CLIENT_NET_OK;
}

clientNetErr recvMsg(int _client_socket, int _maxMsgSize, char *_msgFromServer,  int *_bytesRecieved)
{
	
	*_bytesRecieved = recv(_client_socket, _msgFromServer, _maxMsgSize, 0);
	/* 
		while check != full { do recv }
		PROTOCOL_ERR ProtocolCheck(PackedMessage _packMsg, size_t _rcvMsgSize); 
		PROTOCOL_MSG_FULL,
    	PROTOCOL_MSG_PART
		tmp_buffer = malloc(size = read_bytes)
		memcpy(client_buffer+end of last part, tmp) agregate information
	*/
	if (*_bytesRecieved < 0) { return RECV_FAIL; }
	
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

void destroyListOfGroups(List *_grps)
{
	ListItr_ForEach(ListItrBegin(_grps), ListItrEnd(_grps),destroyGroup, NULL);
}

int destroyGroup(void *_element,void *_context)
{
	if (_element == NULL) { return 0; }
	free( (Group*)_element );
}
/* getters und setters*/
int getClientSocket(Client *_client)
{
	if (_client == NULL) { return -1; }
	return _client->m_clientSocket;
}

void setCLientName(Client *_client, char *_name)
{
	strcpy(_client->m_name, _name); 
}

char *getClientName(Client *_client)
{
	return _client->m_name;
}

List *getClientsConnectedGroups(Client *_client)
{
	return _client->m_connectedGroups;
}

int getGroupChatId(Group *_group)
{
	return _group->m_chatId;
}
void setGroupChatId(Group *_group, int _id)
{
	_group->m_chatId = _id;
}

int getClientNumOfGroups(Client *_client)
{
	return _client->m_numOfGroups;
} 

void setGroupName(Group *_gr, char *_grName)
{
	strcpy(_gr->m_grpName, _grName);
}
void setGroupAddr(Group *_gr, char *_grIp, int _grPort)
{
	initAddr(&(_gr->m_groupAddr), _grIp, _grPort);
}
