#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int openChat(char  *_grpIp, int _grpPort)
{
    
    char listener[100];
    char sender[100];
    sprintf(listener, "mate-terminal --geometry 20x20+500+250 --command ./listen.out 224.0.0.1 8080");
    sprintf(sender, "mate-terminal --geometry 10x10+500+500 --command ./send.out %s %d", _grpIp, _grpPort);
    system(listener);
    system(sender);
    return 0;
}
int main(int argc, char const *argv[])
{
    openChat("224.0.0.1", 8080);
    
    return 0;
}


