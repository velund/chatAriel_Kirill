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
#include "chatOpener.h"
#define RECIEVE_BUFFER_SIZE 256
#define IPV4_ADDR_LEN 25
#define MAX_GOUPS_VECTOR_MSG_SIZE 500
#define MAX_MSG_GROUP_REQ_SIZE 256
#define GRPOUPS_VECTOR_INIT_SIZE 5
#define GROUPS_VECTOR_ENLARGE 5

typedef enum
{
	BAD,
	GOOD
}TREATED;
/* assist funccs declarations */
TREATED treatServerResponse(MSG_RESPONSE _unpckdMsg);
CLIENT_APP_ERR loginRegister(MSG_TYPE _msgtypeToSend, Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR checkStartTalkParams(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR groupsRequest(Client *_client, MSG_TYPE _msgtypeToSend, MSG_TYPE _msgtypeToRecv, char *_grpName, MSG_RESPONSE *_unpckdMsg);
CLIENT_APP_ERR recieveMsgGroupReq(Client *_client, char *_ip, int *_port, MSG_RESPONSE _expected);
CLIENT_APP_ERR sendMessageGroupReq(Client *_client, MSG_TYPE _msgType,  char *_grpName);
CLIENT_APP_ERR checkGroupsParams(Client *_client, char *_grpName);
CLIENT_APP_ERR  addGroupToClientNet(Client *_client, char *_grpName, char *_grpIp, int _grpPort, Group **_group );

CLIENT_APP_ERR sendGroupsVectrorReq(Client *_client);
CLIENT_APP_ERR recieveGroupsVector(Client *_client, Vector *_vector);
CLIENT_APP_ERR closeTheChat(Client *_client, char *_grpName);
CLIENT_APP_ERR closeAllChats(Client *_client);

CLIENT_APP_ERR recvLogoutReq(Client *_client);
CLIENT_APP_ERR sendLogoutReq(Client *_client);
void showAllGroups(Client *_client);
void destroyAllClientsGroups(Client *_client);
/* end assist funcs declarations*/

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
	if ( (loginRegister(LOGIN_REQ, _client, _userName, _userPass)) != CLIENT_APP_OK) { return LOGIN_FAIL; }
	setCLientName(_client, _userName);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR LogOutClient(Client *_client, char* _userName)
{
	CLIENT_APP_ERR check;
	if ( (check =  checkStartTalkParams(_client, _userName,  _userName )) != CLIENT_APP_OK ) { return check; }
	if ( (check =sendLogoutReq(_client)) !=CLIENT_APP_OK) { return check; } 
	if ( (check =recvLogoutReq(_client)) !=CLIENT_APP_OK ) { return check; } 
	closeAllChats(_client);
	destroyAllClientsGroups(_client);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR createGroup(Client *_client, char *_grpName)
{
	char grpIp[IPV4_ADDR_LEN];
	int grpPort;
	CLIENT_APP_ERR check;
	Group *group;
	if ( ( check = checkGroupsParams(_client, _grpName) ) != CLIENT_APP_OK )
	{
		return check;
	}
	if ( (check = sendMessageGroupReq(_client, GROUP_CREATE_REQ, _grpName)) !=  CLIENT_APP_OK) { return check; }
	if ( (check = recieveMsgGroupReq(_client, grpIp, &grpPort, GROUP_CREATED)) != CLIENT_APP_OK ) { return check; }
	if ( (addGroupToClientNet(_client, _grpName, grpIp, grpPort, &group)) !=  CLIENT_APP_OK) 
	{ return GROUP_ADDING_TO_CLIENT_NET_FAILURE; }

	if ( (openChat(grpIp, grpPort, getClientName(_client), _grpName, getGroupChatId(group))) != CHAT_OPENER_SUCCESS) 
	{ 
		return CLIENT_APP_OPEN_CHAT_FAIL; 
	} 
	return CLIENT_APP_OK;
} 

Vector* getGroups(Client *_client)
{
	CLIENT_APP_ERR check;

	Vector *groupsList = VectorCreate(GRPOUPS_VECTOR_INIT_SIZE, GROUPS_VECTOR_ENLARGE);

	if ( (check = sendGroupsVectrorReq(_client)) != CLIENT_APP_OK ) { return NULL; } 
	
	if ( (check = recieveGroupsVector(_client, groupsList)) != CLIENT_APP_OK ) { return NULL; } 
	
	return groupsList;
}

CLIENT_APP_ERR joinGroup(Client *_client, char *_grpName)
{
	CLIENT_APP_ERR check;
	char grpIp[IPV4_ADDR_LEN];
	int grpPort;
	
	Group *group;
	if ( (check = sendMessageGroupReq(_client, GROUP_JOIN_REQ, _grpName)) != CLIENT_APP_OK) { return check; }
	if ( (check = recieveMsgGroupReq(_client, grpIp, &grpPort, GROUP_JOINED)) != CLIENT_APP_OK) { return check; }
	
	if ( (addGroupToClientNet(_client, _grpName, grpIp, grpPort, &group)) !=  CLIENT_APP_OK) 
	{ return GROUP_ADDING_TO_CLIENT_NET_FAILURE; }

	if ( (openChat(grpIp, grpPort, getClientName(_client), _grpName, getGroupChatId(group))) != CHAT_OPENER_SUCCESS) 
	{ 
		return CLIENT_APP_OPEN_CHAT_FAIL; 
	}
	return CLIENT_APP_OK;

} /*packJoinGroup, send(TCP), recv, unpack if ok call connectToGroup ((clientNet))*/ 
CLIENT_APP_ERR leaveGroup(Client *_client, char *_grpName)
{
	char recieveBuffer[RECIEVE_BUFFER_SIZE];
	int bytesRecieved;
	showAllGroups(_client);
	closeTheChat(_client, _grpName);
	
	if ( (sendMessageGroupReq(_client, GROUP_LEAVE_REQ, _grpName)) != CLIENT_APP_OK) { return CLIENT_APP_GRP_LEAVE_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, recieveBuffer, &bytesRecieved) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	if ( ProtocolGetMsgResponse(recieveBuffer) != GROUP_LEFT ) { return GROUP_LEAVE_MSG_RESP_FAILURE; }
		
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
	char recieveBuffer[MAX_MSG_GROUP_REQ_SIZE];
	int bytesRecieved;
	if ( (check =  checkStartTalkParams(_client, _userName,  _userPass )) != CLIENT_APP_OK ) { return check; }
	pckdMsg = ProtocolPackUserDetails(_msgtypeToSend, _userName, _userPass, &msgSize);	

	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, recieveBuffer, &bytesRecieved) != CLIENT_NET_OK ) { return RECVING_FAIL; }
	unpckdMsg = ProtocolUnpackRespMsg(recieveBuffer); 
	if ( (treatServerResponse(unpckdMsg)) != GOOD ) { return CLIENT_APP_LOGIN_OR_REG_FALURE;}
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK; /* TODO: return fail on login fail! */
}

TREATED treatServerResponse(MSG_RESPONSE _unpckdMsg)
{
	switch (_unpckdMsg)
	{	
		/* Register: */
		case USER_CREATED:
			printf("user created successfully\n");
			return GOOD;
	    case USER_EXISTS:
			printf("user exists\n");
			return BAD;
	    case USER_NAME_TOO_SHORT:
			printf("user name too short\n");
			return BAD;
	    case PASS_TOO_SHORT:
			printf("password too short\n");
			return BAD;
		/* Login: */
		case USER_CONNECTED:
			printf("Connected Successfully\n");
			return GOOD;
	    case PASS_INCORRECT:
			printf("incorrect password\n");
			return BAD;
		case USER_NOT_FOUND:
			printf("user not found\n");
			return BAD;
		case GROUP_NOT_EXISTS:
			printf("GROUP_NOT_EXISTS\n");
			return BAD;
		case GROUP_NAME_TOO_SHORT:
			printf("GROUP_NAME_TOO_SHORT\n");
			return BAD;
		case GROUP_LEFT:
			printf("GROUP_LEFT\n");
			return GOOD;
		case GROUP_LEAVE_FAIL:
			printf("GROUP_LEAVE_FAIL\n");
			return BAD;
	    case GEN_ERROR:
			printf("General error\n");
			return BAD;
		case USER_DISCONNECTED:
			printf("USER_DISCONNECTED\n");
			return GOOD;
	
		default:
			printf("unknown response msg\n");
			return BAD;
	}
}
CLIENT_APP_ERR groupsRequest(Client *_client, MSG_TYPE _msgtypeToSend, MSG_TYPE _msgtypeToRecv, char *_grpName, MSG_RESPONSE *_unpckdMsg)
{
	
	size_t msgSize;
	PackedMessage pckdMsg;
	int bytesRecieved;
	if (_grpName == NULL) { return NAME_NULL_POINTER; }
	pckdMsg = ProtocolPackGroupName(_msgtypeToSend, _grpName, &msgSize);
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, pckdMsg, &bytesRecieved) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	if ( (ProtocolGetMsgType(pckdMsg)) != _msgtypeToRecv  ) { return MSG_TYPE_FAIL; }
	*_unpckdMsg = ProtocolUnpackRespMsg(pckdMsg); /* enum in protocol */
	
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
} 

CLIENT_APP_ERR  addGroupToClientNet(Client *_client, char *_grpName, char *_grpIp, int _grpPort, Group **_group )
{
	
	if ( (addGroup(_client, _grpName, _grpIp, _grpPort, _group)) != CLIENT_NET_OK )
	{
		return GROUP_CREATION_FAILURE;
	}
	return 	CLIENT_APP_OK;
}

CLIENT_APP_ERR sendLogoutReq(Client *_client)
{
	PackedMessage pckdMsg;
	size_t msgSize;
	pckdMsg = ProtocolPackLogoutReq(&msgSize);	
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR recvLogoutReq(Client *_client)
{
	MSG_RESPONSE response;
	char recieveBuffer[MAX_MSG_GROUP_REQ_SIZE];
	int bytesRecieved;
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, recieveBuffer, &bytesRecieved) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	response = ProtocolUnpackRespMsg(recieveBuffer);
	if ( (treatServerResponse(response)) != GOOD ) { return LOGOUT_FAILURE; } 
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR sendMessageGroupReq(Client *_client, MSG_TYPE _msgType,  char *_grpName)
{
	PackedMessage pckdMsg;
	size_t msgSize;
	pckdMsg = ProtocolPackGroupName(_msgType, _grpName, &msgSize);
	if (sendMsg(getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR recieveMsgGroupReq(Client *_client, char *_ip, int *_port, MSG_RESPONSE _expected)
{
	char recieveBuffer[MAX_MSG_GROUP_REQ_SIZE];
	int bytesRecieved;
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, recieveBuffer, &bytesRecieved) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	} 
	if ( ProtocolGetMsgResponse(recieveBuffer) != _expected ) { return GROUP_CREATE_MSG_RESP_FAILURE; }
	ProtocolUnpackGroupDetails(recieveBuffer,_ip, _port);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR sendGroupsVectrorReq(Client *_client)
{
	PackedMessage pckdMsg;
	size_t msgSize;
	pckdMsg = ProtocolPackGroupListRequest(&msgSize);
	if (sendMsg( getClientSocket(_client), pckdMsg, msgSize) != CLIENT_NET_OK ) { return SENDING_FAIL; }
	ProtocolPackedMsgDestroy(pckdMsg);
	return CLIENT_APP_OK;
}

CLIENT_APP_ERR recieveGroupsVector(Client *_client, Vector *_vector)
{
	char buffer[MAX_GOUPS_VECTOR_MSG_SIZE];
	int bytesRecieved;
	if (recvMsg(getClientSocket(_client), RECIEVE_BUFFER_SIZE, buffer, &bytesRecieved) != CLIENT_NET_OK ) 
	{ 
		return RECVING_FAIL;
	}
	if ( (ProtocolUnpackGroupList(buffer, _vector)) != PROTOCOL_SUCCESS ) 
	{ 
		return CLIENT_APP_UNPACKING_GRP_VECT_FAILED; 
	} 
	return CLIENT_APP_OK;
	
}

void destroyAllClientsGroups(Client *_client)
{
	destroyListOfGroups(getClientsConnectedGroups(_client));	
}

CLIENT_APP_ERR closeTheChat(Client *_client, char *_grpName)
{
	if ( (closeChat(_grpName)) != CHAT_OPENER_SUCCESS ) { return CLOSE_CHAT_FAILURE; }
	removegroupFromClientsList(_client, _grpName);
	return CLIENT_APP_OK;
}
CLIENT_APP_ERR closeAllChats(Client *_client)
{
	int i;
	char *grpName;
	for (i=0; i< getClientNumOfGroups(_client); i++)
	{
		grpName = getFirstGroupName(_client);
		closeTheChat(_client, grpName);
	}
	return CLIENT_APP_OK;
}

void showAllGroups(Client *_client)
{
	showAllClientsGroups(_client);
}
/*end assist funcs */
