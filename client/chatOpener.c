#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chatOpener.h"

CHAT_OPENER_ERR openChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName)
{
    char listener[100];
    char sender[100];
    sprintf(listener, "gnome-terminal -- ./listen.out %s %d %s", _grpIp, _grpPort, _groupName);
    sprintf(sender, "gnome-terminal -- ./send.out %s %d %s %s", _grpIp, _grpPort, _userName, _groupName);
    
    system(listener);
    system(sender);
    return 0;
}

