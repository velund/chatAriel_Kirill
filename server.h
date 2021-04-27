/*****************************************************************************
* File Name: Generic TCP IPv4 Server
* By: Ariel
* Date: 22/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __SERVER_H__
#define __SERVER_H__

/*---------------------------------- Typedef -------------------------------*/

typedef struct TCPServer TCPServer;

/*---------------------------------- enum ----------------------------------*/

typedef enum TCP_ERROR{
    TCP_SUCCESS,
    TCP_NOT_INITALIZED,
    TCP_SERVER_STOPPED,

    TCP_SELECT_ERR,
    TCP_NO_NEW_CLIENTS,
    TCP_CLIENT_ACCEPT_FAIL,
    TCP_SERVER_FULL,
    TCP_CLIENT_CREATE_FAIL,
    TCP_CLIENT_ADD_FAIL,
    TCP_NEW_CLIENT_CONNECTED,

    TCP_SERVER_EMPTY,
    TCP_CLIENTS_HANDELD,
    TCP_CLIENT_RCV_ERROR,

    TCP_SEND_FAIL
} TCP_ERROR ;

typedef enum SRVR_RUN_ACT{
    SERVER_RUN_CONTINUE,
    SERVER_RUN_STOP
} SRVR_RUN_ACT;

/*---------------------------------- Structs -------------------------------*/

#define IPV4_ADDRESS_SIZE 20

typedef struct ClientInfo{
    int m_clientID;
    int m_clientPort;
    char m_ipv4Address[IPV4_ADDRESS_SIZE];
} ClientInfo;

typedef struct ServerInfo{
    size_t m_port; /* > 1025 */
    char *m_ipv4Address;
    size_t m_maxClientsNum; /* >= 1 */
    size_t m_backLogSize; /* >= 1 */
    size_t m_bufferSize; /* in bytes */
} ServerInfo;

/* --------------- Typedef App Functions --------------- */


/*
 * Description: Action function provided to server initiated every time a new client is connected.
 * Inputs: Server pointer and client info struct.
*/
typedef void (*NewClient)(TCPServer* _server, ClientInfo _newClientInfo, void* _context);


/*
 * Description: Action function provided to server initiated every time a client sends a message to the server.
 * Inputs: Server pointer, client ID, message buffer and size of the message in bytes.
*/
typedef void (*GotMessage)(TCPServer* _server, int _clientID, char* _msg, size_t _msgSize, void* _context);


/*
 * Description: Action function provided to server initiated every time a client closes it's connection to the server.
 * Inputs: Server pointer, client ID.
*/
typedef void (*CloseClient)(TCPServer* _server, int _clientID, void* _context);

/*
 * Description: Action function provided to server initiated every time a fail is taking place.
 * Inputs: Fail from TCP_ERROR enum.
 * Outputs: SERVER_RUN_CONTINUE or SERVER_RUN_STOP returned to the server due to the error.
*/
typedef SRVR_RUN_ACT (*FailOperation)(TCP_ERROR _error, void* _context); /* stop or continue */ 

typedef struct AppFunctions{
    NewClient m_newClientFunc;
    void* m_newClientActContext; /* can be NULL */

    GotMessage m_gotNewMessageFunc;
    void* m_newMsgActContext; /* can be NULL */

    CloseClient m_closeClientFunc;
    void* m_closeClientContext; /* can be NULL */

    FailOperation m_failFunc;
    void* m_failContext; /* can be NULL */
} AppFunctions;


/*-------------------------- Functions declarations -------------------------*/

/*
 * Description: Creates server with given info struct (port, ip, max clients number, backlog size, receive buffer size), and functions.
 * Inputs: server info struct and app funcion struct - as mentioned above.
 * Outputs: TCP server pointer, or NULL if failed.
 * Errors: NULL - if one or more of the given app functions are NULL, OR port is illegal (lower than 1025) or IP is illegal, max clients is < 1, backlog size is < 1, buffer size < 10 bytes.
 *                OR memory allocation fail.
*/
TCPServer *ServerCreate(ServerInfo _serInfo, AppFunctions _appFunc);

/*
 * Description: Destroy the given server.
 * Inputs: Pointer to server pointer
 * Outputs: NONE.
 * Errors: Won't destroy if given server not initalized, or already destroyed.
*/
void ServerDestroy(TCPServer** _server);

/*
 * Description: Run the given server. Runs as long no stop sign given from app FAIL function. Checks for new client connections and treats the connected clients as guided by the app.
 * Inputs: Server pointer
 * Outputs: TCP_NOT_INITALIZED, TCP_SERVER_STOPPED,
 * Errors: TCP_NOT_INITALIZED if given server not initalized.
*/
TCP_ERROR ServerRun(TCPServer* _server);

/*
 * Description: Send a message to server's client.
 * Inputs: Server pointer, client's ID, buffer that holds the message and size of the message to send from the buffer in bytes.
 * Outputs: TCP_SUCCESS on success.
 * Errors: TCP_SEND_FAIL if failed, will initate the app fail function.
*/
TCP_ERROR ServerSend(TCPServer* _server, int _clientID, char* _buffer, size_t _msgSize);



#endif /* __SERVER_H__ */