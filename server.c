
#include <string.h> /* memset() */
#include <netinet/in.h> /* struct sockaddr_in , INADDR_ANY */
#include <arpa/inet.h> /* inet_ntoa() */
#include <sys/socket.h> /* socket() */
#include <unistd.h> /* close() */
#include <sys/select.h> /* select */
#include <stdlib.h> /* size_t, malloc(), free() */

#include <errno.h> /* perror() */
#include <stdio.h> /* perror() */

#include "list.h"
#include "list_itr.h"
#include "server.h"


#define INTERN_SUCCESS 0
#define INTERN_FAIL -1

#define CONTINUE_RUN 0
#define STOP_RUN 1

#define CLOSE_CONNECTION 0 /* message rcv from client */
#define RECEIVE_FAIL -1

#define MAX_CLIENT_DESCRIPTORS 1020
#define MIN_PORT_NUM 1025
#define MAX_PORT_NUM 65535
#define MIN_BACK_LOG 1
#define MIN_CLIENTS_NUM 1
#define MAX_DESCRIPTORS 1020
#define MIN_BUFFER_SIZE 10

/* -------------------------- TCP Server Struct ---------------------------- */

struct TCPServer{
    int m_listenSock;
    size_t m_backLogSize;
    char m_ipv4Address[IPV4_ADDRESS_SIZE];
    size_t m_port;    
    struct sockaddr_in m_serverAddr;

    List* m_clients;
    size_t m_numOfClients;
    size_t m_maxNumOfClients;

    char* m_buffer;
    size_t m_bufferSize;
    fd_set m_connectedSockets;
    fd_set m_readySockets;

    AppFunctions m_appFunc;
} ;

/*---------------------------------- Helper Functions Prototypes ----------------------------------*/

static int ServerGetListenSocket(TCPServer* _newServer);
static void SetPortReusable(int _serverLisSocket); /* can be fail for now */
static int InitTCPServer(TCPServer* _newServer);
static int InitServerParmeters(TCPServer* _newServer, ServerInfo _serInfo);
static int InitServerAppFunc(TCPServer* _newServer, AppFunctions _appFunc);

static void ClientListDestroy(void *_clientSocket);

static int PrepServer(TCPServer* _tcpServer);
static SRVR_RUN_ACT CheckForNewClient(TCPServer* _tcpServer);
static SRVR_RUN_ACT AddNewClient(TCPServer* _tcpServer, size_t _clientSocket, struct sockaddr_in _clientAddr);
static SRVR_RUN_ACT HandleClients(TCPServer* _tcpServer);
static void RemoveClient(TCPServer* _tcpServer, ListItr _clientItr);

/*---------------------------------------- Main Functions ----------------------------------------*/

TCPServer *ServerCreate(ServerInfo _serInfo, AppFunctions _appFunc) 
{
    TCPServer *newTCPServer = malloc(sizeof(TCPServer));
    if(newTCPServer == NULL)
    {
        return NULL;
    }

    if(ServerGetListenSocket(newTCPServer) != INTERN_SUCCESS) /* returns INTERN_SUCCESS or INTERN_FAIL */
    {
        free(newTCPServer);
        return NULL;
    }

    SetPortReusable(newTCPServer->m_listenSock); /* not obligated, can fail  */

    if( InitServerParmeters(newTCPServer, _serInfo) != INTERN_SUCCESS)  /* returns INTERN_SUCCESS or INTERN_FAIL */
    {
        free(newTCPServer);
        return NULL;
    }   

    if( InitTCPServer(newTCPServer) != INTERN_SUCCESS) /* returns INTERN_SUCCESS or INTERN_FAIL */
    {
        free(newTCPServer);
        return NULL;
    }   

    if( InitServerAppFunc(newTCPServer, _appFunc) != INTERN_SUCCESS) /* returns INTERN_SUCCESS or INTERN_FAIL */
    {
        free(newTCPServer);
        return NULL;
    }

    newTCPServer->m_clients = ListCreate(); /* Create list for clients */
    if(newTCPServer->m_clients == NULL)
    {
        free(newTCPServer);
        return NULL;
    }

    newTCPServer->m_buffer = malloc( _serInfo.m_bufferSize );

    FD_ZERO(&newTCPServer->m_connectedSockets);
    FD_SET(newTCPServer->m_listenSock, &(newTCPServer->m_connectedSockets));

    return newTCPServer;
}

void ServerDestroy(TCPServer** _server)
{
    if(_server == NULL || *_server == NULL)
    {
        return;
    }
    ListDestroy( &(*_server)->m_clients,  ClientListDestroy);
    free((*_server)->m_buffer);
    free((*_server)->m_ipv4Address);
    free(*_server);
    
    *_server = NULL;
}

TCP_ERROR ServerRun(TCPServer* _server)
{
    if(_server == NULL)
    {
        return TCP_NOT_INITALIZED;
    }

    while(1)
    {
        if( PrepServer(_server) == INTERN_FAIL ) /* Preps sockets using select() */ /* returns INTERN_SUCCESS or INTERN_FAIL */
        {
            break;
        } 
        if( CheckForNewClient(_server) == SERVER_RUN_STOP)
        {
            break;
        }

        if( HandleClients(_server) == SERVER_RUN_STOP)
        {
            break;
        }
    }

    return TCP_SERVER_STOPPED;
}

TCP_ERROR ServerSend(TCPServer* _server, size_t _clientID, char* _buffer, size_t _msgSize)
{
    size_t sentBytes;
    sentBytes = send(_clientID, _buffer, _msgSize , 0);
    printf("----in server net sent %ld---\n", sentBytes);
    if ( sentBytes < 0)
    {
        _server->m_appFunc.m_failFunc(TCP_SEND_FAIL, _server->m_appFunc.m_failContext);
        return TCP_SEND_FAIL;
    }
    return TCP_SUCCESS;
}

TCP_ERROR ServerClientClose(TCPServer* _server, size_t _clientID)
{
    if(_server == NULL)
    {
        return TCP_NOT_INITALIZED;
    }
    if(close(_clientID) != 0)
    {
        return TCP_CLOSE_FAIL;
    }
    return TCP_SUCCESS;
}




/*---------------------------------------- Helper Functions ----------------------------------------*/


/* ---------- SERVER INIT ---------- */

static int ServerGetListenSocket(TCPServer* _newServer) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    _newServer->m_listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if(_newServer->m_listenSock < 0)
    {
        perror("Server socket failed!");
        return INTERN_FAIL;
    }
    return INTERN_SUCCESS;
}

static void SetPortReusable(int _serverLisSocket)
{
    int reuseOptVal = 1;
    if(setsockopt(_serverLisSocket, SOL_SOCKET, SO_REUSEADDR, &reuseOptVal, sizeof(reuseOptVal)) < 0) /* setting as reusable  */
    {
        perror("Setting port as reusable failed!\n");
    }
}

static int InitServerParmeters(TCPServer* _newServer, ServerInfo _serInfo) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    if(_serInfo.m_port < MIN_PORT_NUM || _serInfo.m_port > MAX_PORT_NUM || _serInfo.m_maxClientsNum < MIN_CLIENTS_NUM || _serInfo.m_backLogSize < MIN_BACK_LOG || _serInfo.m_maxClientsNum > MAX_CLIENT_DESCRIPTORS || _serInfo.m_bufferSize < MIN_BUFFER_SIZE )
    {
        return INTERN_FAIL;
    }
    strcpy( _newServer->m_ipv4Address, _serInfo.m_ipv4Address);
    _newServer->m_port = _serInfo.m_port;
    _newServer->m_numOfClients = 0;
    _newServer->m_maxNumOfClients = _serInfo.m_maxClientsNum;
    _newServer->m_backLogSize = _serInfo.m_backLogSize;
    _newServer->m_bufferSize = _serInfo.m_bufferSize;
    return INTERN_SUCCESS;
}

static int InitTCPServer(TCPServer* _newServer) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    struct sockaddr_in serverAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(_newServer->m_ipv4Address) /*INADDR_ANY*/;
    serverAddr.sin_port = htons(_newServer->m_port);

    if(bind(_newServer->m_listenSock, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Server bind Failed! \n");
        return INTERN_FAIL;
    }

    if(listen(_newServer->m_listenSock, _newServer->m_backLogSize) < 0)
    {
        perror("Server listen failed! \n");
        return INTERN_FAIL;
    }
    return INTERN_SUCCESS;
}


static int InitServerAppFunc(TCPServer* _newServer, AppFunctions _appFunc) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    if(_appFunc.m_gotNewMessageFunc == NULL)
    {
        return INTERN_FAIL;
    }

    _newServer->m_appFunc = _appFunc;
    return INTERN_SUCCESS;
}

/* ---------- SERVER Destroy ---------- */

static void ClientListDestroy(void *_clientSocket)
{
    close(*(size_t*)_clientSocket);
    free(_clientSocket);
}




/* ---------- SERVER RUN ---------- */

static int PrepServer(TCPServer* _tcpServer) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    _tcpServer->m_readySockets = _tcpServer->m_connectedSockets;
    if( select( MAX_DESCRIPTORS, &(_tcpServer->m_readySockets), NULL, NULL, NULL) < 0)
    {
        perror("Select error!");
        _tcpServer->m_appFunc.m_failFunc(TCP_SELECT_ERR, _tcpServer->m_appFunc.m_failContext );
        return INTERN_FAIL;
    }
    return INTERN_SUCCESS;
}



/* --- New Client --- */

static SRVR_RUN_ACT CheckForNewClient(TCPServer* _tcpServer)
{
    struct sockaddr_in clientAddr;
    socklen_t clientAddLen = sizeof(clientAddr);
    size_t clientSocket;
    
    if(FD_ISSET(_tcpServer->m_listenSock, &_tcpServer->m_readySockets) == 0) /* No Clients waiting in queue */
    {
        return SERVER_RUN_CONTINUE; 
    }

    clientSocket = accept(_tcpServer->m_listenSock, (struct sockaddr*) &clientAddr, &clientAddLen); 
    
    if(clientSocket < 0) 
    {
        perror("Client acception failed! \n");
        return _tcpServer->m_appFunc.m_failFunc( TCP_CLIENT_ACCEPT_FAIL, _tcpServer->m_appFunc.m_failContext);
    }
    else if(clientSocket > 0) /* Valid client */
    {
        return AddNewClient(_tcpServer, clientSocket, clientAddr);
    }
}

static SRVR_RUN_ACT AddNewClient(TCPServer* _tcpServer, size_t _clientSocket, struct sockaddr_in _clientAddr)
{
    size_t* newClient;
    ClientInfo newClientInfo;
    
    if(_tcpServer->m_numOfClients >= _tcpServer->m_maxNumOfClients) 
    {
        close(_clientSocket);
        return _tcpServer->m_appFunc.m_failFunc( TCP_SERVER_FULL, _tcpServer->m_appFunc.m_failContext);
    }

    newClient = malloc(sizeof(size_t));
    if(newClient == NULL)
    {
        close(_clientSocket);
        return _tcpServer->m_appFunc.m_failFunc( TCP_CLIENT_CREATE_FAIL, _tcpServer->m_appFunc.m_failContext);
    }
    *newClient = _clientSocket;

    if( ListPushHead(_tcpServer->m_clients, (void*)newClient) != LIST_SUCCESS)
    {
        close(_clientSocket);
        free(newClient);
        return _tcpServer->m_appFunc.m_failFunc( TCP_CLIENT_ADD_FAIL, _tcpServer->m_appFunc.m_failContext);
    }

    FD_SET(_clientSocket, &_tcpServer->m_connectedSockets);
    (_tcpServer->m_numOfClients)++;

    /* Creates client info and sends to NewClient() */
    
    newClientInfo.m_clientID = _clientSocket;
    newClientInfo.m_clientPort = ntohs(_clientAddr.sin_port);
    strcpy( newClientInfo.m_ipv4Address, inet_ntoa(_clientAddr.sin_addr));

    (_tcpServer->m_appFunc.m_newClientFunc)(_tcpServer, newClientInfo, _tcpServer->m_appFunc.m_newClientActContext);
    return SERVER_RUN_CONTINUE;
}

/* --- Handle Client --- */

static SRVR_RUN_ACT HandleClients(TCPServer* _tcpServer)
{
    size_t currentClientSock;
    int readBytes;
    
    ListItr currentItr, endItr, removeClientItr;

    currentItr = ListItrBegin(_tcpServer->m_clients);
    endItr = ListItrEnd(_tcpServer->m_clients);
    
    if(currentItr == endItr)
    {
        return SERVER_RUN_CONTINUE;
    }

    while(currentItr != endItr)
    {
        currentClientSock = *(size_t*)ListItrGet(currentItr);
        if(FD_ISSET(currentClientSock, &_tcpServer->m_readySockets) == 0) /* skip */
        {
            currentItr = ListItrNext(currentItr);
            continue;
        }

        readBytes = recv(currentClientSock, _tcpServer->m_buffer, _tcpServer->m_bufferSize, 0);
        
        switch (readBytes)
        {
            case CLOSE_CONNECTION:
                removeClientItr = currentItr;
                currentItr = ListItrNext(currentItr);

                _tcpServer->m_appFunc.m_closeClientFunc(_tcpServer, currentClientSock, _tcpServer->m_appFunc.m_closeClientContext);  /* should be closed */
                RemoveClient(_tcpServer, removeClientItr);
                break;

            case RECEIVE_FAIL:
                removeClientItr = currentItr;
                currentItr = ListItrNext(currentItr);
                _tcpServer->m_appFunc.m_failFunc(TCP_CLIENT_RCV_ERROR, _tcpServer->m_appFunc.m_failContext);
                _tcpServer->m_appFunc.m_closeClientFunc(_tcpServer, currentClientSock, _tcpServer->m_appFunc.m_closeClientContext);  /* should be closed */
                RemoveClient(_tcpServer, removeClientItr);
                break;

            default:
                _tcpServer->m_appFunc.m_gotNewMessageFunc(_tcpServer, currentClientSock, _tcpServer->m_buffer, (readBytes / sizeof(char)), _tcpServer->m_appFunc.m_newMsgActContext); 
                currentItr = ListItrNext(currentItr);
                break;
        }
    }
    return SERVER_RUN_CONTINUE;
}

static void RemoveClient(TCPServer* _tcpServer, ListItr _clientItr)
{
    void* clientSocketPtr;
    size_t socket;

    clientSocketPtr = (ListItrRemove(_clientItr));
    socket = *(size_t*)clientSocketPtr;

    close(socket);
    free(clientSocketPtr);

    (_tcpServer->m_numOfClients)--;
    FD_CLR( socket, &_tcpServer->m_connectedSockets);
}