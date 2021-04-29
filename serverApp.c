
#include <string.h> /* memcpy() */
#include <stdlib.h> /* malloc(), free() */ 
#include <stdio.h>
#include <unistd.h> /* usleep() */

#include "HashMap.h"
#include "server.h"
#include "serverApp.h"
#include "protocol.h"
#include "userMng.h"
#include "groupMng.h"
#include "vector.h"

#include "serverAPPUI.h"

#define USLEEP_WAIT 200
#define TEMP_BUF_SIZE 200

#define USER_NAME_LENGTH 20
#define USER_PASS_LENGTH 20
#define GROUP_NAME_LENGTH 20

#define CLIENT_HASH_SIZE 50
#define USER_MNG_SIZE 50
#define GROUPS_MAX 100

struct ServerApp{
    TCPServer* m_serverNet;
    UserMng* m_userMng;
    GroupMng* m_groupMng;
    HashMap* m_connectedClients;
} ;

typedef struct ConnectedClient{
    ClientInfo m_clientInfo;
    char userName[USER_NAME_LENGTH];
    char* m_tempBuffer; /* initalized as NULL */ 
} ConnectedClient;

typedef enum APP_INTERN_ERR{
    SUCCESS,
    GEN_FAIL,
    MSG_SAVED,
    APP_MSG_TYPE_ERR,
    MSG_READ_ERR,

    APP_USER_EXISTS,
    USER_REGISTERED,
    USER_FAIL,

    /* New client() */
    CLIENT_ADD_FAIL,
    CLIENT_ADD_SUCCESS
} APP_INTERN_ERR;

/*---------------------------------------- Helper Functions ----------------------------------------*/

/* HashMap */
size_t HashClientKey(void* _clientID);
int HashClientEquality(void* _clientID1, void* _clientID2);
static void ConnClientHashKeyDestroy(void *_key);
static void ConnClientHashDestroy(void *_connClient);

/* Server App Create */
static AppFunctions CreateFunc(ServerApp* _serverApp);

/* Create New connected client  */
static ConnectedClient* ConnClientCreate(ClientInfo _clientInfo);
static void ConnClientDestroy(ConnectedClient** _client);

/* New Message() */
static APP_INTERN_ERR CheckMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR TreatMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static void SendAppResp( ServerApp* _serverApp, int _clientID, MSG_TYPE _type, MSG_RESPONSE _response);

static APP_INTERN_ERR RegisterUser( ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR LoginUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR AddUsernameToClient(ServerApp* _serverApp, int _clientID, char* _userName); /* ON login */
static APP_INTERN_ERR LogoutUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);

static APP_INTERN_ERR CreateGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR JoinGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR LeaveGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);

static void SendGroupDetails(ServerApp* _serverApp, MSG_RESPONSE _res, int _clientID, char* _ip, int _port);

/* NewClient() */
static APP_INTERN_ERR AddConnectedServerClient(ServerApp* _serverApp, ClientInfo _newClientInfo);


/*---------------------------------------- Main Functions ----------------------------------------*/

ServerApp* ServerAppCreate(ServerInfo _serInfo)
{
    ServerApp* newApp;
    AppFunctions appFunc;

    newApp = malloc(sizeof(ServerApp));
    if(newApp == NULL)
    {
        return NULL;
    }

    appFunc = CreateFunc(newApp);

    newApp->m_serverNet = ServerCreate(_serInfo, appFunc);
    if(newApp->m_serverNet == NULL)
    {
        free(newApp);
        return NULL;
    }

    newApp->m_connectedClients = HashMapCreate(CLIENT_HASH_SIZE, HashClientKey, HashClientEquality);
    if(newApp->m_connectedClients == NULL)
    {
        ServerDestroy(&newApp->m_serverNet);
        free(newApp);
        return NULL;
    }

    newApp->m_userMng = UserMngCreate(USER_MNG_SIZE);
    if(newApp->m_userMng == NULL)
    {
        HashMapDestroy(&newApp->m_connectedClients, NULL, NULL);
        ServerDestroy(&newApp->m_serverNet);
        free(newApp);
        return NULL;
    }

    newApp->m_groupMng = GroupMngCreate(GROUPS_MAX);
    if(newApp->m_groupMng == NULL)
    {
        UserMngDestroy(&newApp->m_userMng);
        HashMapDestroy(&newApp->m_connectedClients, NULL, NULL);
        ServerDestroy(&newApp->m_serverNet);
        free(newApp);
        return NULL;
    }

    return newApp;
}


void ServerAppDestroy(ServerApp** _app)
{
    if(_app == NULL || *_app == NULL)
    {
        return;
    }
    ServerDestroy(&(*_app)->m_serverNet);
    HashMapDestroy( &(*_app)->m_connectedClients, ConnClientHashKeyDestroy, ConnClientHashDestroy);
    UserMngDestroy(&(*_app)->m_userMng);
}

int ServerAppRun(ServerApp* _app)
{
    UIServerStart();
    ServerRun(_app->m_serverNet);
}




/*---------------------------------------- TCP Server Functions ----------------------------------------*/

void NewClientFunc(TCPServer* _server, ClientInfo _newClientInfo, void* _serverApp) /* Adds the new client to hashmap and sends back success or fail to client */
{
    if(AddConnectedServerClient( (ServerApp*)_serverApp, _newClientInfo) != CLIENT_ADD_SUCCESS)
    {
        if ( ServerClientClose(_server, _newClientInfo.m_clientID) != TCP_SUCCESS )
        {
            UIClientConnFail(_newClientInfo);
        }
        /*SendAppResp(_serverApp, _newClientInfo.m_clientID, CONN_REC, CONN_FAIL);*/
        return;
    }
    /*SendAppResp(_serverApp, _newClientInfo.m_clientID, CONN_REC, CONN_SUCCESS);*/
    UIClientConnSuccess(_newClientInfo);
}



void GotMessageFunc(TCPServer* _server, int _clientID, char* _msg, size_t _msgSize, void* _serverApp) /* TODO: combined half message with prev one if needed */
{
    char tempBuffer[TEMP_BUF_SIZE]; /* Needed if msg streamed in few parts from TCP */

    if ( CheckMsg ((ServerApp*)_serverApp, _clientID, _msg, _msgSize)  != SUCCESS ) 
    {
        return;
    }
    TreatMsg( (ServerApp*)_serverApp, _clientID, _msg, _msgSize); /* TODO:  list of groups */
}  

void CloseClientFunc(TCPServer* _server, int _clientID, void* _serverApp) /* TODO: client will send leave group */
{
    ConnectedClient* client;

    if( HashMapFind (((ServerApp*)_serverApp)->m_connectedClients, (void*)&_clientID, (void**)&client) != MAP_SUCCESS )
    {
        return;
    }
    ConnClientDestroy(&client);
    
}  

SRVR_RUN_ACT FailFunc(TCP_ERROR _error, void* _serverApp) /* TODO: treat error? how? */ 
{
    
}

/*---------------------------------------- TCP Server Helper Functions ----------------------------------------*/

static APP_INTERN_ERR AddConnectedServerClient(ServerApp* _serverApp, ClientInfo _newClientInfo)
{
    int *newClientSockKey;
    ConnectedClient *newClientVal;

    newClientSockKey = malloc(sizeof(int));
    if(newClientSockKey == NULL)
    {
        return CLIENT_ADD_FAIL;
    }
    *newClientSockKey = _newClientInfo.m_clientID;

    newClientVal = ConnClientCreate(_newClientInfo);
    if(newClientVal == NULL)
    {
        free(newClientSockKey);
        return CLIENT_ADD_FAIL;
    }

    if( HashMapInsert( ((ServerApp*)(_serverApp))->m_connectedClients, (void*)newClientSockKey, (void*)newClientVal) != MAP_SUCCESS )
    {
        free(newClientSockKey);
        ConnClientDestroy(&newClientVal);
        return CLIENT_ADD_FAIL;
    }
    return CLIENT_ADD_SUCCESS;
}


/*---------------------------------------- Helper Functions ----------------------------------------*/


static AppFunctions CreateFunc(ServerApp* _serverApp)
{
    AppFunctions appFunc;

    appFunc.m_newClientFunc = NewClientFunc;
    appFunc.m_newClientActContext = (void*)_serverApp;

    appFunc.m_gotNewMessageFunc = GotMessageFunc;
    appFunc.m_newMsgActContext = (void*)_serverApp;

    appFunc.m_closeClientFunc = CloseClientFunc;
    appFunc.m_closeClientContext = (void*)_serverApp;

    appFunc.m_failFunc = FailFunc;
    appFunc.m_failContext = (void*)_serverApp;

    return appFunc;
}

static ConnectedClient* ConnClientCreate(ClientInfo _clientInfo)
{
    ConnectedClient* newClient;

    newClient = malloc(sizeof(ConnectedClient));
    if(newClient == NULL)
    {
        return NULL;
    }

    newClient->m_clientInfo = _clientInfo;
    newClient->m_tempBuffer = NULL;
    return newClient;
}

static void ConnClientDestroy(ConnectedClient** _client)
{
    if(_client == NULL || *_client == NULL)
    {
        return;
    }
    if((*_client)->m_tempBuffer != NULL)
    {
        free((*_client)->m_tempBuffer);
    }
    free(*_client);
    *_client = NULL;
}


/*---------------------------------------- Server Helper Functions ----------------------------------------*/

static APP_INTERN_ERR CheckClientBuffer(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize, char* _tempBuffer)
{
    ConnectedClient* client;

    if( HashMapFind(_serverApp->m_connectedClients, (void*)&_clientID, (void**)&client) != MAP_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, MSG_TYPE_ERR, GEN_ERROR);
        return GEN_FAIL;
    }

    if(client->m_tempBuffer != NULL)
    {
        
    }
}


static APP_INTERN_ERR CheckMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    ConnectedClient* client;

    if(ProtocolCheck (_msg, _msgSize) == PROTOCOL_MSG_FULL )
    {
        return SUCCESS;
    }

    if( HashMapFind(_serverApp->m_connectedClients, (void*)&_clientID, (void**)&client) != MAP_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, MSG_TYPE_ERR, GEN_ERROR);
        return GEN_FAIL;
    }

    client->m_tempBuffer = malloc(_msgSize * sizeof(char));
    memcpy(client->m_tempBuffer, _msg, _msgSize);

    return MSG_SAVED;
}

static APP_INTERN_ERR TreatMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    MSG_TYPE currentMsgType;
    
    currentMsgType = ProtocolGetMsgType(_msg);
    
    switch (currentMsgType)
    {
    case REG_REQ: /* V */
        RegisterUser(_serverApp, _clientID, _msg, _msgSize);
        break;
    
    case LOGIN_REQ: /* V */
        LoginUser(_serverApp, _clientID, _msg, _msgSize);
        break;

    case LOGOUT_REQ: /* V */
        LogoutUser(_serverApp, _clientID, _msg, _msgSize);
        break;

    
    case GROUP_LIST_REQ: 
        
        break;

    case GROUP_CREATE_REQ:
        CreateGroup(_serverApp, _clientID, _msg, _msgSize);
        break;

    case GROUP_JOIN_REQ:
        JoinGroup(_serverApp, _clientID, _msg, _msgSize);
        break;

    case GROUP_LEAVE_REQ:
        LeaveGroup(_serverApp, _clientID, _msg, _msgSize);
        break;


    default:
        SendAppResp(_serverApp, _clientID, MSG_TYPE_ERR, GEN_ERROR);
        return MSG_TYPE_ERR;
        break;
    }
}

static APP_INTERN_ERR SendGroupList(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    Vector* nameVector;

    nameVector = VectorCreate(GROUPS_MAX);
}


/* -------- Users -------- */

static APP_INTERN_ERR RegisterUser( ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR addResult;
    char userName[USER_NAME_LENGTH], userPass[USER_PASS_LENGTH];


    if( ProtocolUnpackUserDetails(_msg, userName, userPass) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, REG_REC, GEN_ERROR);
        UIMsgRcvErr();
        return;
    }
    
    addResult = UserMngAdd(_serverApp->m_userMng, userName, userPass);

    switch (addResult)
    {
    case USER_MNG_USER_EXISTS:
        SendAppResp(_serverApp, _clientID, REG_REC, USER_EXISTS);
        UIUserExistsRegErr(userName);
        break;
    case USER_MNG_USER_ADDED:
        SendAppResp(_serverApp, _clientID, REG_REC, USER_CREATED);
        UIUserCreated(userName);
        break;
    default:
        SendAppResp(_serverApp, _clientID, REG_REC, GEN_ERROR);
        break;
    }
    
}

static APP_INTERN_ERR LoginUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR loginRes;
    char userName[USER_NAME_LENGTH], userPass[USER_PASS_LENGTH];

    if( ProtocolUnpackUserDetails(_msg, userName, userPass) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, LOGIN_REC, GEN_ERROR);
        UIMsgRcvErr();
        return MSG_READ_ERR;
    }

    loginRes = UserMngConnect(_serverApp->m_userMng, userName, userPass);

    switch (loginRes)
    {
    case USER_MNG_USER_NOT_FOUND:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, USER_NOT_FOUND);
        break;
    
    case USER_MNG_PASS_INCORRECT:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, PASS_INCORRECT);
        break;

    case USER_MNG_ALREADY_CONNECT:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, USER_ALREADY_CONNECTED);
        break;
    case USER_MNG_SUCCESS:
        AddUsernameToClient(_serverApp, _clientID, userName);
        SendAppResp(_serverApp, _clientID, LOGIN_REC, USER_CONNECTED);
        break;

    default:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, GEN_ERROR);
        break;
    }
}

static APP_INTERN_ERR AddUsernameToClient(ServerApp* _serverApp, int _clientID, char* _userName)
{
    ConnectedClient* client;

    if( HashMapFind (_serverApp->m_connectedClients, (void*)&_clientID, (void**)&client) != MAP_SUCCESS)
    {
        return GEN_FAIL;
    }

    strcpy(client->userName, _userName);

    return SUCCESS;
}


static APP_INTERN_ERR LogoutUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR logoutRes;
    char userName[USER_NAME_LENGTH];

    if( ProtocolUnpackUserName(_msg, userName) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, LOGOUT_REC, GEN_ERROR);
        UIMsgRcvErr();
        return MSG_READ_ERR;
    }

    logoutRes = UserMngDisconnect(_serverApp->m_userMng, userName);

    switch (logoutRes)
    {
    case USER_MNG_USER_NOT_FOUND:
        SendAppResp(_serverApp, _clientID, LOGOUT_REC, USER_NOT_FOUND);
        break;
    
    case USER_MNG_SUCCESS:
        SendAppResp(_serverApp, _clientID, LOGOUT_REC, USER_DISCONNECTED);
        break;

    default:
        SendAppResp(_serverApp, _clientID, LOGOUT_REC, GEN_ERROR);
        break;
    }
}


/* -------- GROUPS -------- */

static APP_INTERN_ERR CreateGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR createRes;
    char groupName[GROUP_NAME_LENGTH];
    char groupIP[IPV4_ADDRESS_SIZE];
    int groupPort;

    if( ProtocolUnpackGroupName(_msg, groupName) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, GROUP_CREATE_REQ, GEN_ERROR);
        UIMsgRcvErr();
        return MSG_READ_ERR;
    }

    createRes = GroupMngAdd(_serverApp->m_groupMng, groupName, groupIP, &groupPort);

    switch (createRes)
    {
    case GROUP_MNG_SUCCESS:
        SendGroupDetails(_serverApp, GROUP_CREATED,_clientID, groupIP, groupPort);
        UIGroupCreated(groupName, groupIP, groupPort);
        break;

    default:
        SendAppResp(_serverApp, _clientID, GROUP_CREATE_REC, GROUP_CREATE_FAIL);
        break;
    }
}


static APP_INTERN_ERR JoinGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR joinRes;

    char groupName[GROUP_NAME_LENGTH];
    char groupIP[IPV4_ADDRESS_SIZE];
    int groupPort;

    if( ProtocolUnpackGroupName(_msg, groupName) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, GROUP_JOIN_REC, GEN_ERROR);
        UIMsgRcvErr();
        return MSG_READ_ERR;
    }
    
    joinRes = GroupMngJoin(_serverApp->m_groupMng, groupName, groupIP, &groupPort);

    switch (joinRes)
    {
    case GROUP_MNG_SUCCESS: 
        SendGroupDetails(_serverApp, GROUP_CREATED,_clientID, groupIP, groupPort);
        UIGroupJoined(_clientID, groupName, groupIP, groupPort);
        break;

    default:
        SendAppResp(_serverApp, _clientID, GROUP_JOIN_REC, GROUP_JOIN_FAIL);
        break;
    }
}

static APP_INTERN_ERR LeaveGroup(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR leaveRes;
    char groupName[GROUP_NAME_LENGTH];

    if( ProtocolUnpackGroupName(_msg, groupName) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, GROUP_LEAVE_REC, GEN_ERROR);
        UIMsgRcvErr();
        return MSG_READ_ERR;
    }

    leaveRes = GroupMngLeave(_serverApp->m_groupMng, groupName);

    switch (leaveRes)
    {
    case GROUP_MNG_SUCCESS:
        SendAppResp(_serverApp, _clientID, GROUP_LEAVE_REC, GROUP_LEFT);
        UIGroupLeft(_clientID, groupName);
        break;

    default:
        SendAppResp(_serverApp, _clientID, GROUP_LEAVE_REC, GROUP_LEAVE_FAIL);
        break;
    }
}   


/*---------------------------------------- Send Functions ----------------------------------------*/


static void SendGroupDetails(ServerApp* _serverApp, MSG_RESPONSE _res,int _clientID, char* _ip, int _port)
{
    PackedMessage msg;
    size_t pckMsgSize;

    msg = ProtocolPackGroupDetails(GROUP_CREATE_REC, _res, _ip, _port, &pckMsgSize);

    ServerSend(_serverApp->m_serverNet,_clientID, msg, pckMsgSize);
    printf("step\n");
    ProtocolPackedMsgDestroy(msg);
}

static void SendAppResp( ServerApp* _serverApp, int _clientID, MSG_TYPE _type, MSG_RESPONSE _response)
{
    PackedMessage msg;
    size_t pckMsgSize;

    msg = ProtocolPackRespMsg(_type, _response, &pckMsgSize);
    
    ServerSend(_serverApp->m_serverNet,_clientID, msg, pckMsgSize);

    ProtocolPackedMsgDestroy(msg);
}


/*---------------------------------------- Hash Functions ----------------------------------------*/

size_t HashClientKey(void* _clientID)
{
    return *(size_t*)_clientID;
}

int HashClientEquality(void* _clientID1, void* _clientID2)
{
    if( *(size_t*)_clientID1 == *(size_t*)_clientID2  )
    {
        return 1;
    }
    return 0;
}

static void ConnClientHashKeyDestroy(void *_key)
{
    free(_key);
}

static void ConnClientHashDestroy(void *_connClient)
{
    ConnClientDestroy( (ConnectedClient**) &_connClient);
}