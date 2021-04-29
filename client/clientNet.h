#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <stdlib.h> /*  size_t */


#define SERVER_IP "192.168.1.109"
#define SERVER_PORT 8080 

typedef struct Client Client;
typedef struct Group Group;

typedef enum clientNetErr
{
	CLIENT_NET_OK,
	SEND_FAIL,
	RECV_FAIL,
	SOCKET_OPENING_FAIL,
	CONNECTION_FAIL,

	INCORRECT_ARGS_OF_ADD_GROUP,
	GROUP_MALLOC_FAILED,

	GROUP_NOT_CREATED
} clientNetErr;

Client *connectClient();
void disconnect(Client **_client);
clientNetErr sendMsg(int _client_socket, char *_messageFromClient, size_t _msgLen);
clientNetErr recvMsg(int _client_socket, int _maxMsgSize, char *_msgFromServer, int *_bytesRecieved);
clientNetErr addGroup(Client *_client, char *_grpName, char *_ip, int _port);
/* getter */
int getClientSocket(Client *_client );
void setCLientName(Client *_client, char *_name);
void setGroupName(Group *_gr, char *_grName);
void setGroupAddr(Group *_gr, char *_grIp, int _grPort);
char *getClientName(Client *_client);
#endif /* __CLIENTNET_H__ */
