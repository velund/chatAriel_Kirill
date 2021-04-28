#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int openChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName)
{
    char listener[100];
    char sender[100];
    sprintf(listener, "gnome-terminal -- ./listen.out %s %d %s", _grpIp, _grpPort, _groupName);
    sprintf(sender, "gnome-terminal -- ./send.out %s %d %s %s", _grpIp, _grpPort, _userName, _groupName);
    
    system(listener);
    system(sender);
    return 0;
}

/*
int main(int argc, char const *argv[])
{
    openChat("224.0.0.1", 8080, "Arik", "Group1");

    openChat("224.0.0.1", 8080, "Kirill", "Group1");
    
    return 0;
}

*/
