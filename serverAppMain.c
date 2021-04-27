#include "stdio.h"

#include "serverApp.h"

#define BACK_LOG_SIZE 10
#define BUFFER_SIZE 200
#define IP_ADDRESS "127.0.0.1"
#define MAX_CLIENTS_NUM 20
#define PORT 8080

int main(void)
{
    ServerApp* myApp;
    ServerInfo myInfo;
    
    myInfo.m_backLogSize = 10;
    myInfo.m_bufferSize = BUFFER_SIZE;
    myInfo.m_ipv4Address = IP_ADDRESS;
    myInfo.m_maxClientsNum = MAX_CLIENTS_NUM;
    myInfo.m_port = PORT;

    printf("Server Start: \n");
    myApp = ServerAppCreate(myInfo);

    ServerAppRun(myApp);

    ServerDestroy(&myApp);
}