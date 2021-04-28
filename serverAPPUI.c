
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