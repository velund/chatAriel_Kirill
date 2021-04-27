#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <stdlib.h> /*  size_t */


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080 

typedef struct Client Client;
typedef struct Group Group;

typedef enum clientNetErr
{
	OK,
	SEND_FAIL,
	RECV_FAIL,
	SOCKET_OPENING_FAIL,
	CONNECTION_FAIL


} clientNetErr;

Client *connectClient();
void disconnect(Client **_client);
clientNetErr sendMsg(int _client_socket, char *_messageFromClient, size_t _msgLen);
clientNetErr recvMsg(int _client_socket, int _maxMsgSize, char *_msgFromServer);
/* getter */
int getClientSocket(Client *_client );
#endif /* __CLIENTNET_H__ */
