#ifndef __CHATOPENER_H__
#define __CHATOPENER_H__

typedef enum
{
	CHAT_OPENER_SUCCESS,
	OPEN_CHAT_FAIL,
	CLOSE_CHAT_GR_NM_NOT_FOUND_IN_FILE,
	FILE_NOT_FOUND,
	FILE_READ_MALLOC_FAIL,
	FILE_CANT_BE_OPENED,
	CHAT_CLOSING_FAILURE
}CHAT_OPENER_ERR;
CHAT_OPENER_ERR openChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName, int _chatId);
CHAT_OPENER_ERR closeChat(char *_grpName);

#endif /* __CHATOPENER_H__ */