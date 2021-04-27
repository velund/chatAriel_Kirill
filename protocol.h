/*****************************************************************************
* File Name: Protocol.h - Message protocol
* Date: 26/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*****************************************************************************
* Description:
* Message protocol used by server app and client app.
* 
* Order of use:
* When sending a message: use ProtoclPack()
* When receiving a message: 1) use ProtocolCheck() to check if message received fully
*                           2) use ProtocolGetMsgType() to check for action
*                           3) use ProtocolUnpackUserDetails() if REG_REQ or LOGIN_REQ
                               use ProtocolUnpackUserName() if LOGOUT_REQ
                            4) use ProtocolPackedMsgDestroy() to free memory from given packed message
*****************************************************************************/

/*---------------------------------- Typedef --------------------------------*/

typedef char* PackedMessage ;

/*---------------------------------- Enum --------------------------------*/

typedef enum PROTOCOL_ERR {
    PROTOCOL_SUCCESS, 
    PROTOCOL_PACK_ERR,
    PROTOCOL_MSG_NOT_INITALIZED,
    
    PROTOCOL_MSG_FULL,
    PROTOCOL_MSG_PART
} PROTOCOL_ERR;

typedef enum MSG_TYPE{
    REG_REQ,
    LOGIN_REQ,
    LOGOUT_REQ,

    REG_REC,
    LOGIN_REC,
    LOGOUT_REC,

    GROUP_CREATE,
    GROUP_JOIN,
    GROUP_LEAVE,

    MSG_TYPE_ERR,
    MAX_MSG_TYPE
} MSG_TYPE ;

typedef enum MSG_RESPONSE{
    USER_CREATED,
    USER_EXISTS,
    USER_NAME_TOO_SHORT,
    PASS_TOO_SHORT,

    PASS_INCORRECT,
    USER_NOT_FOUND,
    USER_CONNECTED,

    USER_DISCONNECTED,
    

    GEN_ERROR,

    MSG_RES_MAX
} MSG_RESPONSE ;

/*-------------------------- Functions declarations -------------------------*/


/*
 * Description: Pack given user name and password
 * Inputs: MSG Type, _userName, and password, pointer to save the new size of message to
 * Outputs: Packed message
 * Errors: returns NULL if given paramaters not initalized.
*/
PackedMessage ProtocolPackUserDetails(MSG_TYPE _type, char* _userName, char* _userPass, size_t *_pckMsgSize) ;

/*
 * Description: Pack given user name and password
 * Inputs: MSG Type, _userName, pointer to save the new size of message to
 * Outputs: Packed message
 * Errors: returns NULL if given paramaters not initalized.
*/
PackedMessage ProtocolPackUserName(MSG_TYPE _type, char* _userName, size_t *_pckMsgSize) ;


/*
 * Description: Pack given response message
 * Inputs: MSG Type, response type, pointer to save the new size of message to
 * Outputs: Packed message
 * Errors: returns NULL if given paramaters not initalized.
*/
PackedMessage ProtocolPackRespMsg(MSG_TYPE _type, MSG_RESPONSE _response, size_t *_pckMsgSize);


/*
 * Description: Destroy and frees memory from packed message
 * Inputs: Packed Message pointer
 * Outputs: NONE.
 * Errors: Won't free if given message is not initalized.
*/
void ProtocolPackedMsgDestroy(PackedMessage _msg);

/*
 * Description: Get message type from given packed message.
 * Inputs: Packed Message pointer
 * Outputs: Message type enum or MSG_TYPE_ERR if given message not initalized.
 * Errors: MSG_TYPE_ERR if given message not initalized.
*/
MSG_TYPE ProtocolGetMsgType(PackedMessage _packedMsg);

/*
 * Description: Unpack user details from packed message. @ warning - do not use unless Get message type returns REG_REQ or LOGIN_REQ!
 * Inputs: Packed Message pointer, pointer to save user name to, pointer to save user pass to. @ warning - allocate at least 15 bytes of memory for each.
 * Outputs: PROTOCOL_MSG_NOT_INITALIZED, PROTOCOL_SUCCESS
 * Errors: PROTOCOL_MSG_NOT_INITALIZED - if given parameters not initalized
*/
PROTOCOL_ERR ProtocolUnpackUserDetails(PackedMessage _packedMsg, char* _userName, char* _userPass);

/*
 * Description: Unpack user name from packed message. @ warning - do not use unless Get message type returns LOGOUT_REQ!
 * Inputs: Packed Message pointer, pointer to save user name to. @ warning - allocate at least 15 bytes of memory for Username.
 * Outputs: PROTOCOL_MSG_NOT_INITALIZED, PROTOCOL_SUCCESS
 * Errors: PROTOCOL_MSG_NOT_INITALIZED - if given parameters not initalized
*/
PROTOCOL_ERR ProtocolUnpackUserName(PackedMessage _packedMsg, char* _userName);

/*
 * Description: Unpack Given packed response message
 * Inputs: Packed Message pointer
 * Outputs: MSG_RESPONSE type of response or GEN_ERR if give message not initalized
 * Errors: GEN_ERR if give message not initalized
*/
MSG_RESPONSE ProtocolUnpackRespMsg(PackedMessage _packedMsg);

/*
 * Description: Check if message received fully
 * Inputs: Packed Message pointer, size of received message in bytes
 * Outputs: PROTOCOL_MSG_FULL, PROTOCOL_MSG_PART, PROTOCOL_MSG_NOT_INITALIZED
 * Errors: PROTOCOL_MSG_NOT_INITALIZED - given message too small or not initalized 
*/
PROTOCOL_ERR ProtocolCheck(PackedMessage _packMsg, size_t _rcvMsgSize);

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
PROTOCOL_ERR ProtocolEncrypt(PackedMessage _Msg, size_t _msgSize); /* TODO: Encrypt type? */ 

/*
 * Description:
 * Inputs:
 * Outputs:
 * Errors:
*/
PROTOCOL_ERR ProtocolDecrypt(PackedMessage _Msg, size_t _msgSize);






#endif /* __PROTOCOL_H__ */

