#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__

#include "clientNet.h"

typedef enum
{
    CLIENT_APP_OK,
    SENDING_FAIL,
    RECVING_FAIL,
    UNPACKING_FAIL,
    CLIENT_NULL_POINTER,
    NAME_NULL_POINTER,
    PASSWORD_NULL_POINTER,
    /* groups */
    CLIENT_GROUP_ARGS_FAILURE,
    GROUP_CREATE_MSG_RESP_FAILURE,
    GROUP_CREATION_FAILURE,
    GROUP_ADDING_TO_CLIENT_NET_FAILURE,
    GROUP_JOINING_FAILURE,
    GROUP_LEAVING_FAILURE,
    MSG_TYPE_FAIL,
    UNPACK_GROUP_DET_FAILURE
}CLIENT_APP_ERR; 

Client *createClientConnection();
void destroyClientConnection(Client **_client);
CLIENT_APP_ERR registerClient(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR LoginClient(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR LogOutClient(Client *_client, char* _userName);
/*
    
*/
CLIENT_APP_ERR createGroup(Client *_client, char *_grpName); /*if ok joinGroup()  */
void getGroups(); /* call showGroups((UI)) */
CLIENT_APP_ERR joinGroup(Client *_client, char *_grpName); /*packJoinGroup, send(TCP), recv, unpack if ok call connectToGroup ((clientNet))*/ 
CLIENT_APP_ERR leaveGroup(Client *_client, char *_grpName); 
/* 
    kills mains 
    call disconnectFromGroup((clientNet) 
    packLeaveGroup, send(TCP), recv, unpack) 
*/




#endif /* __CLIENTAPP_H__ */