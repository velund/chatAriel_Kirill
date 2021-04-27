
#include <string.h> /* memcpy() */
#include <stdlib.h> /* malloc(), free() */ 
#include <stdio.h>

#include "HashMap.h"
#include "server.h"
#include "serverApp.h"
#include "protocol.h"
#include "userMng.h"


struct ServerApp{
    TCPServer* m_serverNet;
    UserMng* m_userMng;
    HashMap* m_connectedClients;
} ;

typedef struct ConnectedClient{
    int m_clientID;
    char* m_tempBuffer; /* initalized as NULL */ 
} ConnectedClient;


typedef enum APP_INTERN_ERR{
    SUCCESS,
    MSG_SAVED,
    APP_MSG_TYPE_ERR,
    MSG_READ_ERR,

    APP_USER_EXISTS,
    USER_REGISTERED,
    USER_FAIL
} APP_INTERN_ERR;

#define USER_NAME_LENGTH 20
#define USER_PASS_LENGTH 20

#define CLIENT_HASH_SIZE 50
#define USER_MNG_SIZE 50

/*---------------------------------------- Helper Functions ----------------------------------------*/

size_t HashClientKey(void* _clientID);
int HashClientEquality(void* _clientID1, void* _clientID2);
static void ConnClientHashKeyDestroy(void *_key);
static void ConnClientHashDestroy(void *_connClient);

static AppFunctions CreateFunc(ServerApp* _serverApp);

static ConnectedClient* ConnClientCreate(int _clientID);
static void ConnClientDestroy(ConnectedClient** _client);

static APP_INTERN_ERR CheckMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR TreatMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);

static APP_INTERN_ERR RegisterUser( ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR LoginUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);
static APP_INTERN_ERR LogoutUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize);

static void SendAppResp( ServerApp* _serverApp, int _clientID, MSG_TYPE _type, MSG_RESPONSE _response);


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
        free(newApp);
        return NULL;
    }

    newApp->m_userMng = UserMngCreate(USER_MNG_SIZE);
    if(newApp->m_userMng == NULL)
    {
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
    ServerRun(_app->m_serverNet);
}




/*---------------------------------------- TCP Server Functions ----------------------------------------*/

void NewClientFunc(TCPServer* _server, ClientInfo _newClientInfo, void* _serverApp)
{
    
}

void GotMessageFunc(TCPServer* _server, int _clientID, char* _msg, size_t _msgSize, void* _serverApp) /* TODO: combined half message with prev one if needed */
{
    if ( CheckMsg ((ServerApp*)_serverApp, _clientID, _msg, _msgSize)  != SUCCESS ) 
    {
        return;
    }
    if ( TreatMsg( (ServerApp*)_serverApp, _clientID, _msg, _msgSize) )
    {

    }

}  

void CloseClientFunc(TCPServer* _server, int _clientID, void* _serverApp)
{

}

SRVR_RUN_ACT FailFunc(TCP_ERROR _error, void* _serverApp)
{
    
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



static ConnectedClient* ConnClientCreate(int _clientID)
{
    ConnectedClient* newClient;
    newClient = malloc(sizeof(ConnectedClient));
    if(newClient == NULL)
    {
        return NULL;
    }
    newClient->m_clientID = _clientID;
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

void PrintAll(char* _buffer, size_t _size)
{
    int i;
    printf("%d, %d, %d" ,_buffer[0], _buffer[1], _buffer[2]);
    for(i = 3; i < _size; i++)
    {
        putchar(_buffer[i]);
        
        
    }
    putchar('\n');
}

static APP_INTERN_ERR CheckMsg(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    ConnectedClient* client;
    printf("size: %d\n ", _msgSize);
    PrintAll(_msg, 15);
    if(ProtocolCheck (_msg, _msgSize) == PROTOCOL_MSG_FULL )
    {
        return SUCCESS;
    }

    HashMapFind(_serverApp->m_connectedClients, (void*)&_clientID, (void**)&client);

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
    case REG_REQ:
        RegisterUser(_serverApp, _clientID, _msg, _msgSize);
        break;
    
    case LOGIN_REQ:
        LoginUser(_serverApp, _clientID, _msg, _msgSize);
        break;

    case LOGOUT_REQ:
        LogoutUser(_serverApp, _clientID, _msg, _msgSize);
        break;

    case GROUP_CREATE:
        /* code */
        break;

    case GROUP_JOIN:
        /* code */
        break;

    case GROUP_LEAVE:
        /* code */
        break;


    default:
        return MSG_TYPE_ERR;
        break;
    }
}

static APP_INTERN_ERR RegisterUser( ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR addResult;
    char userName[USER_NAME_LENGTH], userPass[USER_PASS_LENGTH];

    printf("RegisterUnpack\n");

    if( ProtocolUnpackUserDetails(_msg, userName, userPass) != PROTOCOL_SUCCESS)
    {
        SendAppResp(_serverApp, _clientID, REG_REC, GEN_ERROR);
        return;
    }

    printf("%s %s\n",userName, userPass);
    
    addResult = UserMngAdd(_serverApp->m_userMng, userName, userPass);

    switch (addResult)
    {
    case USER_MNG_USER_EXISTS:
        SendAppResp(_serverApp, _clientID, REG_REC, APP_USER_EXISTS);
        break;
    case USER_MNG_USER_ADDED:
        SendAppResp(_serverApp, _clientID, REG_REC, USER_CREATED);
        break;
    default:
        SendAppResp(_serverApp, _clientID, REG_REC, GEN_ERROR);
        break;
    }
    printf("RegisterUnpack\n");
}

static APP_INTERN_ERR LoginUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR loginRes;
    char userName[USER_NAME_LENGTH], userPass[USER_PASS_LENGTH];

    if( ProtocolUnpackUserDetails(_msg, userName, userPass) != PROTOCOL_SUCCESS)
    {
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
    
    case USER_MNG_SUCCESS:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, USER_CONNECTED);
        break;

    default:
        SendAppResp(_serverApp, _clientID, LOGIN_REC, GEN_ERROR);
        break;
    }
    
}

static APP_INTERN_ERR LogoutUser(ServerApp* _serverApp, int _clientID, char* _msg, size_t _msgSize)
{
    USER_MNG_ERR logoutRes;
    char userName[USER_NAME_LENGTH];

    if( ProtocolUnpackUserName(_msg, userName) != PROTOCOL_SUCCESS)
    {
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

static void SendAppResp( ServerApp* _serverApp, int _clientID, MSG_TYPE _type, MSG_RESPONSE _response)
{
    PackedMessage msg;
    size_t pckMsgSize;

    msg = ProtocolPackRespMsg(_type, _response, &pckMsgSize);
    PrintAll(msg, pckMsgSize); /* print */
    
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