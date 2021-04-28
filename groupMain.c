
#include <stdio.h>
#include <stdlib.h>

#include "group.h"

#define GROUP_IP "224.0.0.1"
#define PORT 8080

int main(void)
{
    Group* myGroup;
    char myName[] = "A12312";

    myGroup = GroupCreate(myName, GROUP_IP, PORT);
    if(myGroup == NULL)
    {
        printf("Group Error\n");
        exit(1);
    }

    printf("Group IP %s, Port %d \n", GROUP_IP, PORT);
}