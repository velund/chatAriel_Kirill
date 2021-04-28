
#include "clientApp.h"

#include <stdio.h>

int main(void)
{
    Client* myClient;

    myClient = createClientConnection();
    if(myClient == NULL)
    {
        printf("Create Error\n");
    }
    registerClient(myClient, "Kirill", "1234565"); /* 2+ 1+ 6 + 1 + 5 = 15*/

    LoginClient(myClient, "Kirill", "1234565");

    createGroup(myClient, "myGroup");

    return 0;
}
