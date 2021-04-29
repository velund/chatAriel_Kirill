#ifndef __CHATOPENER_H__
#define __CHATOPENER_H__
typedef enum
{
    OPEN_CHAT_SUCCESS,
    OPEN_CHAT_FAIL
}CHAT_OPENER_ERR;
CHAT_OPENER_ERR openChat(char  *_grpIp, int _grpPort, char* _userName, char* _groupName);
#endif /* __CHATOPENER_H__ */