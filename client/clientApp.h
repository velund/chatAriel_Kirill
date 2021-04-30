#ifndef __CLIENTAPP_H__
#define __CLIENTAPP_H__

#include "clientNet.h"
#include "vector.h"

typedef enum
{
    CLIENT_APP_OK,
    SENDING_FAIL,
    RECVING_FAIL,
    UNPACKING_FAIL,
    CLIENT_NULL_POINTER,
    NAME_NULL_POINTER,
    PASSWORD_NULL_POINTER,
    LOGIN_FAIL,
    CLIENT_APP_LOGIN_OR_REG_FALURE,
    CLIENT_APP_LOGOUT_FAILURE,
    /* groups */
    CLIENT_GROUP_ARGS_FAILURE,
    GROUP_CREATE_MSG_RESP_FAILURE,
    GROUP_CREATION_FAILURE,
    GROUP_ADDING_TO_CLIENT_NET_FAILURE,
    GROUP_JOINING_FAILURE,
    GROUP_LEAVING_FAILURE,
    MSG_TYPE_FAIL,
    UNPACK_GROUP_DET_FAILURE,
    CLIENT_APP_GRP_LEAVE_FAIL,
    /* chat */
    CLIENT_APP_OPEN_CHAT_FAIL,
    /* groups vector recv */
    CLIENT_APP_UNPACKING_GRP_VECT_FAILED,
    /* logout */
    LOGOUT_FAILURE
}CLIENT_APP_ERR; 

Client *createClientConnection();
void destroyClientConnection(Client **_client);
CLIENT_APP_ERR registerClient(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR LoginClient(Client *_client, char* _userName, char* _userPass);
CLIENT_APP_ERR LogOutClient(Client *_client, char* _userName);
/*
    
*/
CLIENT_APP_ERR createGroup(Client *_client, char *_grpName); /*if ok joinGroup()  */
Vector* getGroups(Client *_client);
/*CLIENT_APP_ERR getGroups(Client *_client); */
CLIENT_APP_ERR joinGroup(Client *_client, char *_grpName); /*packJoinGroup, send(TCP), recv, unpack if ok call connectToGroup ((clientNet))*/ 
CLIENT_APP_ERR leaveGroup(Client *_client, char *_grpName); 
/* 
    kills mains 
    call disconnectFromGroup((clientNet) 
    packLeaveGroup, send(TCP), recv, unpack) 
*/




#endif /* __CLIENTAPP_H__ */