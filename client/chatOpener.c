#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chatOpener.h"

CHAT_OPENER_ERR openChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName, int _chatId)
{

	char geometryListener[13];
	char geometrySender[13];
	char listener[100];
	char sender[100];
	sprintf(geometryListener, "20x10+%d+250",_chatId*100);
   	sprintf(geometrySender, "20x10+%d+500",_chatId*100);
	sprintf(listener, "gnome-terminal --geometry %s -- ./listen.out %s %d %s",geometryListener, _grpIp, _grpPort, _groupName);
	sprintf(sender, "gnome-terminal --geometry %s -- ./send.out %s %d %s %s",geometrySender, _grpIp, _grpPort, _userName, _groupName);
	
	system(listener);
	system(sender);
	return 0;

}

CHAT_OPENER_ERR closeChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName, int _chatId)
{

}
