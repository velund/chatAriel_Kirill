
#include "protocol.h"

typedef enum CLIENT_ACT{
    /* Menu 1 */
    ACT_REG,
    ACT_LOG,
    ACT_EXIT,

    /* Menu 2 */
    ACT_CREATE_GRP,
    ACT_JOIN_GRP,
    ACT_LEAVE_GRP,
    ACT_LOGOUT,

    ACT_ERR,
    ACT_SUCCESS
} CLIENT_ACT ;


/* ACT_REG, ACT_LOG, ACT_EXIT */
CLIENT_ACT ClientUIStartMenu();

CLIENT_ACT ClientUIGetUserDetails(char* _userName, char* _passWord);

void ClientUIErr(MSG_RESPONSE _err);


CLIENT_ACT ClientUIMenu2();

CLIENT_ACT ClientUIGetGroupName(char* _groupName);
int showGroupList(void* _element, void* _context);
