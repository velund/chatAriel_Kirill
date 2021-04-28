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
#include "../vector.h"
#define RECIEVE_BUFFER_SIZE 256
#define IPV4_ADDR_LEN 25
/* assist funccs */
void treatServerResponse(MSG_RESPONSE _unpckdMsg);
CLIENT_APP_ERR loginRegister(MSG_TYPE _msgtypeToSend, Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR checkStartTalkParams(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR groupsRequest(Client *_client, MSG_TYPE _msgtypeToSend, MSG_TYPE _msgtypeToRecv, char *_grpName, MSG_RESPONSE *_unpckdMsg);
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
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	treatServerResponse(unpckdMsg);
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR sendMessage()
{

}

CLIENT_APP_ERR createGroup(Client *_client, char *_grpName)
{
	MSG_RESPONSE unpckdMsg;
	PackedMessage pckdMsg;
	size_t msgSize;
	CLIENT_APP_ERR check;
	char grpIp[IPV4_ADDR_LEN];
	int grpPort;
	/* TODO: sendMsg(), recv+unpack()  */
	if ( ( check = checkGroupsParams(_client, _grpName) ) != CLIENT_APP_OK )
	{
		return check;
	}
	pckdMsg = ProtocolPackGroupName(GROUP_CREATE_REQ, _grpName, &msgSize);
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	if ( ProtocolGetMsgResponse(pckdMsg) != GROUP_CREATED ) { return GROUP_CREATE_MSG_RESP_FAILURE; }
	ProtocolUnpackGroupDetails(pckdMsg, grpIp, &grpPort);
	ProtocolPackedMsgDestroy(pckdMsg);
	if ( (addGroupToClientNet(_client, _grpName, grpIp, grpPort)) !=  CLIENT_APP_OK) 
	{ return GROUP_ADDING_TO_CLIENT_NET_FAILURE; }

	openChat(grpIp, grpPort);

	return CLIENT_APP_OK;
} /*if ok joinGroup()  */
void getGroups(Client *_client)
{
	Vector *groupsList = VectorCreate(10, 10);
	PackedMessage pckdMsg;
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	}
	ProtocolUnpackGroupList(groupsList);
	showGroups(groupsList); /* UI function */


	/*showGroups(_client);*/
} /* call showGroups((UI)) */
CLIENT_APP_ERR joinGroup(Client *_client, char *_grpName)
{
	MSG_RESPONSE unpckdMsg;
	CLIENT_APP_ERR check;
	if ( (check = groupsRequest(_client, GROUP_JOIN_REQ, GROUP_JOIN_REC, _grpName, &unpckdMsg)) != CLIENT_APP_OK )
	{ return  GROUP_JOINING_FAILURE;}

	return CLIENT_APP_OK;

} /*packJoinGroup, send(TCP), recv, unpack if ok call connectToGroup ((clientNet))*/ 
CLIENT_APP_ERR leaveGroup(Client *_client, char *_grpName)
{
	MSG_RESPONSE unpckdMsg;
	CLIENT_APP_ERR check;
	if ( (check = groupsRequest(_client, GROUP_LEAVE_REQ, GROUP_LEAVE_REC, _grpName, &unpckdMsg) ) != CLIENT_APP_OK )
	{ return  check;} 
	killMains();
	return CLIENT_APP_OK;
}

/* assist funcs */
CLIENT_APP_ERR checkStartTalkParams(Client *_client, char* _userName, char* _userPass)
{
	if (_client == NULL) { return CLIENT_NULL_POINTER; }
	if ( _userName == NULL ) { return NAME_NULL_POINTER; }
	if ( _userPass == NULL ) { return PASSWORD_NULL_POINTER; }
	return CLIENT_APP_OK;
} 

CLIENT_APP_ERR checkGroupsParams(Client *_client, char *_grpName)
{
	if (_client == NULL || _grpName == NULL) { return CLIENT_GROUP_ARGS_FAILURE; }
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR loginRegister(MSG_TYPE _msgtypeToSend, Client *_client, char* _userName, char* _userPass)
{
	MSG_RESPONSE unpckdMsg;
	PackedMessage pckdMsg;
	size_t msgSize;
	CLIENT_APP_ERR check;
	if ( (check =  checkStartTalkParams(_client, _userName,  _userPass )) != CLIENT_APP_OK ) { return check; }
	pckdMsg = ProtocolPackUserDetails(_msgtypeToSend, _userName, _userPass, &msgSize);	
	printf("size %d\n", msgSize);

	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != CLIENT_NET_OK ) { return RECVING_FAIL; }
	unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	treatServerResponse(unpckdMsg);
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

void treatServerResponse(MSG_RESPONSE _unpckdMsg)
{
	switch (_unpckdMsg)
	{	
		/* Register: */
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
		/* Login: */
		case USER_CONNECTED:
			printf("Connected Successfully\n");
			break;
	    case PASS_INCORRECT:
		printf("incorrect password\n");
			break;
			
	    case GEN_ERROR:
		printf("General error\n");
			break;
	
		default:
			printf("unknown response msg\n");
			break;
	}
}
CLIENT_APP_ERR groupsRequest(Client *_client, MSG_TYPE _msgtypeToSend, MSG_TYPE _msgtypeToRecv, char *_grpName, MSG_RESPONSE *_unpckdMsg)
{
	
	size_t msgSize;
	PackedMessage pckdMsg;
	if (_grpName == NULL) { return NAME_NULL_POINTER; }
	pckdMsg = ProtocolPackGroupName(_msgtypeToSend, _grpName, &msgSize);
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	if ( (ProtocolGetMsgType(pckdMsg)) != _msgtypeToRecv  ) { return MSG_TYPE_FAIL; }
	*_unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
} 

CLIENT_APP_ERR  addGroupToClientNet(Client *_client, char *_grpName, char *_grpIp, int _grpPort )
{
	PROTOCOL_ERR check;
	if ( (addGroup(_client, _grpName, _grpIp, _grpPort)) != CLIENT_NET_OK )
	{
		return GROUP_CREATION_FAILURE;
	}
	return 	CLIENT_APP_OK;
}
/*end assist funcs */
