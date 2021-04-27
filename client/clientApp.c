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
#include "clientApp.h"
#include "clientNet.h"
#include "../protocol.h"
#define RECIEVE_BUFFER_SIZE 100
/* assist funccs */
void treatServerResponse(MSG_RESPONSE _unpckdMsg);
CLIENT_APP_ERR loginRegister(MSG_TYPE _msgtype, Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR checkStartTalkParams(Client *_client, char* _userName, char* _userPass);
/* end assist funcs */

Client *createClientConnection()
{
	Client *client = connectClient(); 
	if (client == NULL) { return NULL; }
	return client;
}

void destroyClientConnection(Client **_client)
{
	if (_client == NULL) { return; }
	disconnect(_client);
}

CLIENT_APP_ERR registerClient(Client *_client, char* _userName, char* _userPass)
{
	return loginRegister(REG_REQ, _client, _userName, _userPass);
}

CLIENT_APP_ERR LoginClient(Client *_client, char* _userName, char* _userPass)
{
	return loginRegister(LOGIN_REQ, _client, _userName, _userPass);
}

CLIENT_APP_ERR LogOutClient(Client *_client, char* _userName)
{
	MSG_RESPONSE unpckdMsg;
	PackedMessage pckdMsg;
	size_t msgSize;
	CLIENT_APP_ERR check;
	if ( (check =  checkStartTalkParams(_client, _userName,  _userName )) != CLIENT_APP_OK ) { return check; }
	pckdMsg = ProtocolPackUserName(LOGOUT_REQ, _userName, &msgSize);	
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != OK ) 
	{ 
		return RECVING_FAIL;
	} 
	unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	treatServerResponse(unpckdMsg);
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

/*CLIENT_APP_ERR createGroup(char *_grpName)
{
	
}

void getGroups()
{
	showAllGroups();
}*/

/* assist funcs */

CLIENT_APP_ERR checkStartTalkParams(Client *_client, char* _userName, char* _userPass)
{
	if (_client == NULL) { return CLIENT_NULL_POINTER; }
	if ( _userName == NULL ) { return NAME_NULL_POINTER; }
	if ( _userPass == NULL ) { return PASSWORD_NULL_POINTER; }
	return OK;
} 

CLIENT_APP_ERR loginRegister(MSG_TYPE _msgtype, Client *_client, char* _userName, char* _userPass)
{
	MSG_RESPONSE unpckdMsg;
	PackedMessage pckdMsg;
	size_t msgSize;
	CLIENT_APP_ERR check;
	if ( (check =  checkStartTalkParams(_client, _userName,  _userPass )) != CLIENT_APP_OK ) { return check; }
	pckdMsg = ProtocolPackUserDetails(_msgtype, _userName, _userPass, &msgSize);	
	printf("size %d\n", msgSize);

	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != OK ) { return RECVING_FAIL; }
	unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	treatServerResponse(unpckdMsg);
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

void treatServerResponse(MSG_RESPONSE _unpckdMsg)
{
	switch (_unpckdMsg)
	{
		case USER_CREATED:
			printf("user created successfully\n");
			break;
	    case USER_EXISTS:
			printf("user exists\n");
			break;
	    case USER_NAME_TOO_SHORT:
		printf("user name too short\n");
			break;
	    case PASS_TOO_SHORT:
		printf("password too short\n");
			break;
	    case PASS_INCORRECT:
		printf("incorrect password\n");
			break;
	    case GEN_ERROR:
		printf("reneral error\n");
			break;
		default:
			printf("unknown response msg\n");
			break;
	}
}
/*end assist funcs */


/*createGroup(grp name) */
	/* 
		get ip port 

	*/
/*joinGroup*/
	/*connect to group*/
/*leaveGroup*/
	/*close*/