
#include <stdio.h>

#include "server.h"

void UIClientConnSuccess(ClientInfo _info)
{
    printf("Client connection SUCCESS: ID %d, on IP %s, port %d\n", _info.m_clientID, _info.m_ipv4Address, _info.m_clientPort);
}

void UIClientConnFail(ClientInfo _info)
{
    printf("Client connection FAIL: ID %d, on IP %s, port %d\n", _info.m_clientID, _info.m_ipv4Address, _info.m_clientPort);
}

void UIServerStart()
{
    printf("Server start running: \n");
}

void UIMsgRcvErr()
{
    printf("Error receiving and unpacking message! \n");
}

void UIUserExistsRegErr(char *_name)
{
    printf("Client tried to register with existing username %s\n", _name);
}

void UIUserCreated(char *_name)
{
    printf("Client registered with username %s\n", _name);
}

void UIGroupCreated(char *_name, char* _ip, int _port)
{
    printf("New group '%s' created with IP %s and port %d\n", _name, _ip, _port);
}

void UIGroupJoined(int _clientID, char *_name, char* _ip, int _port)
{
    printf("Client %d joined group '%s' with IP %s and port %d\n", _clientID, _name, _ip, _port);
}

void UIGroupLeft(int _clientID, char *_name)
{
    printf("Client %d left group '%s'\n", _clientID, _name);
}