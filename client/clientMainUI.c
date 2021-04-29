
#include "clientApp.h"
#include "clientUI.h"

#include <stdio.h>

#define USER_NAME_SIZE 15
#define PASS_SIZE 15

int main(void)
{
    Client* myClient;
    CLIENT_ACT action;
    CLIENT_APP_ERR log = -1;
    char userName[USER_NAME_SIZE];
    char pass[PASS_SIZE];
    char groupName[USER_NAME_SIZE];

    myClient = createClientConnection();
    if(myClient == NULL)
    {
        printf("Create Error\n");
        exit(1);
    }

    while (log != CLIENT_APP_OK) /* stay here until logged in */
    {
        action = ClientUIStartMenu();

        switch (action)
        {
        case ACT_REG:
            ClientUIGetUserDetails(userName, pass);
            registerClient(myClient, userName, pass);
            action = 0;
            action = ClientUIStartMenu();
            break;
        case ACT_LOG:
            ClientUIGetUserDetails(userName, pass);
            log = LoginClient(myClient, userName, pass);

            break;
        case ACT_EXIT:
            destroyClientConnection(&myClient);
            exit(0);
            break;
        }
    }
    
    
    action = -1;
    while(action != ACT_LOGOUT)
    {
        action =  ClientUIMenu2();
        switch (action)
        {
        case ACT_CREATE_GRP:
            ClientUIGetGroupName(groupName);
            createGroup(myClient, groupName);
            break;
        case ACT_JOIN_GRP: /* TODO: show list of groups */
            ClientUIGetGroupName(groupName);
            printf("Asked to join group %s\n", groupName);
            joinGroup(myClient, groupName);
            break;
        case ACT_LEAVE_GRP:
            
            break;
        case ACT_LOGOUT: /* TODO: proper exit */
            destroyClientConnection(&myClient);
            exit(0);
            break;
        }
    }
    


    return 0;
}
