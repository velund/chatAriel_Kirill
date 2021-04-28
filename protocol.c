
#include <stdlib.h> /* malloc() */
#include <string.h> /* memcpy(), strlen() */
#include <arpa/inet.h> /*  */
#include <stdio.h> /* sprintf() */ 

#include "string.h"
#include "protocol.h"

/*
TODO: Encrypt, List of groups
*/
/*
    @WARNING:
        Packed message length parameter is original message size + header size + lengths!

    -- USER ---
    UserDetailsPackedMsg =
    [MSG_TYPE][Whole Msg Length][UserNameLength][UserName(No '\0')][PassLength][Pass(No '\0')]

    UserNamePackMsg = 
    [MSG_TYPE][Whole Msg Length][UserNameLength][UserName(No '\0')]

    ResponsePackMsg = 
    [MSG_TYPE][Whole Msg Length][MSG_RESPONSE]

    -- GROUP ---
    GroupName
    [MSG_TYPE][Whole Msg Length][GroupNameLength][GroupName(No '\0')]

    GroupIPPort - sends port as string + sends confirm message
    [MSG_TYPE][Whole Msg Length][CREATED/FAIL/JOINED/FAIL][IPV4Length][IPV4(No '\0')][PORTLENGTH][PORT(No '\0')]

    GroupList
    [MSG_TYPE][Whole Msg Length][GROUP_LIST_SUCCESS/GROUP_LIST_FAIL][GroupNumber][Len][Group][Len][Group]...
*/

typedef char Byte;

#define HEAD_BYTES 2 
#define LENGTH_BYTES 1
#define MSG_RSP_BYTES 1
#define PORT_SIZE 8

#define MAX_CHAR_NUM 255
#define GROUP_NAME_SIZE 15


/* ------------------------- Helper Functions Prototypes ------------------------- */



/* ------------------------- Main Functions ------------------------- */


MSG_RESPONSE ProtocolGetMsgResponse(PackedMessage _packedMsg)
{
    MSG_RESPONSE response;

    if(_packedMsg == NULL)
    {
        return GEN_ERROR;
    }

    return (MSG_RESPONSE)(_packedMsg[2]);
}

PackedMessage ProtocolPackGroupList( Vector* _groupList, size_t *_pckMsgSize)
{
    PackedMessage packedMsg;
    size_t msgSize = 3, groupNum = 0, i = 4; /* firstLen */
    char* groupName;

    if(_groupList == NULL || _pckMsgSize == NULL)
    {
        return NULL;
    }

    packedMsg = malloc(sizeof(Byte) * 250); /* alloc size?? */
    if(packedMsg == NULL)
    {
        return NULL;
    }

    packedMsg[0] = (Byte)GROUP_LIST_REC;

    packedMsg[2] = (Byte)GROUP_LIST_SUCCESS;

    while(VectorRemove(_groupList, (void**)&groupName) == VECTOR_SUCCESS)
    {
        packedMsg[i] = strlen(groupName);
        strncpy(&packedMsg[i + 1], groupName, packedMsg[i]);
        i += (1 + packedMsg[i]);
        groupNum++;
    }

    msgSize = (Byte)(i - 1);
    packedMsg[1] = (Byte)msgSize;
    packedMsg[3] = (Byte)groupNum;

    return packedMsg;
}
/* [MSG_TYPE][Whole Msg Length][GROUP_LIST_SUCCESS/GROUP_LIST_FAIL][GroupNumber][Len][Group][Len][Group]... */

PROTOCOL_ERR ProtocolUnpackGroupList(PackedMessage _packedMsg, size_t msgSize, Vector* _saveListTo)
{
    size_t i = 5; /* first char of first name */
    char groupName[GROUP_NAME_SIZE];

    if(_saveListTo == NULL || _packedMsg == NULL || msgSize < 4)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }
    /* add \0 */
    while(i < msgSize)
    {
        strncpy(groupName, &_packedMsg[i], _packedMsg[i - 1]);

        i += _packedMsg[i - 1];
    }
}

PackedMessage ProtocolPackUserDetails(MSG_TYPE _type, char* _userName, char* _userPass, size_t *_pckMsgSize)
{
    PackedMessage packedMsg;
    Byte nameLen, passLen;

    if(_type >= MAX_MSG_TYPE || _userName == NULL || _userPass == NULL)
    {
        return NULL;
    }

    nameLen = strlen(_userName);
    passLen = strlen(_userPass);

    packedMsg = malloc( sizeof(Byte) * HEAD_BYTES + ((nameLen + passLen) * sizeof(char)) + 2 * LENGTH_BYTES * sizeof(Byte));
    if(packedMsg == NULL)
    {
        return NULL;
    }    

    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + nameLen + passLen + 2);


    packedMsg[2] = nameLen;
    strncpy(&packedMsg[3], _userName, nameLen);

    packedMsg[3 + nameLen] = passLen;
    strncpy(&packedMsg[3 + nameLen + 1], _userPass, passLen);

    *_pckMsgSize = 2 + nameLen + passLen + 2; /* in Bytes! */

    return packedMsg;
}


PackedMessage ProtocolPackGroupDetails(MSG_TYPE _type, char* _ipv4Addr, int _port, size_t *_pckMsgSize) /* Sends group details on success only! */
{
    PackedMessage packedMsg;
    Byte addrLen, portLen;
    char portStr[PORT_SIZE];

    if(_type >= MAX_MSG_TYPE || _ipv4Addr == NULL || _type != GROUP_CREATED || _type != GROUP_JOINED)
    {
        return NULL;
    }

    addrLen = strlen(_ipv4Addr);

    sprintf(portStr, "%d", _port);
    portLen = strlen(portStr);

    packedMsg = malloc( sizeof(Byte) * HEAD_BYTES + ((addrLen + portLen) * sizeof(char)) + 2 * LENGTH_BYTES * sizeof(Byte) + MSG_RSP_BYTES * sizeof(Byte));
    if(packedMsg == NULL)
    {
        return NULL;
    }    

    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + addrLen + portLen + 2 * LENGTH_BYTES);
    packedMsg[2] = _type;

    packedMsg[3] = addrLen;
    strncpy(&packedMsg[4], _ipv4Addr, addrLen);

    packedMsg[4 + addrLen] = portLen;
    strncpy(&packedMsg[4 + addrLen + 1], portStr, portLen);

    *_pckMsgSize = 2 + addrLen + portLen + 2 * LENGTH_BYTES + MSG_RSP_BYTES; /* in Bytes! */

    return packedMsg;
}

PROTOCOL_ERR ProtocolUnpackGroupDetails(PackedMessage _packedMsg, char* _ipv4Addr, int* _port)
{
    char addrLen, portLen;
    char portStr[PORT_SIZE];

    if(_packedMsg == NULL || _ipv4Addr == NULL || _port == NULL)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }
    if(_packedMsg[2] != GROUP_CREATED && _packedMsg[2] != GROUP_JOINED)
    {
        return PROTOCOL_UNPACK_GROUP_ERR;
    }

    addrLen = _packedMsg[3];
    portLen = _packedMsg[4 + addrLen];

    strncpy(_ipv4Addr, &_packedMsg[4], addrLen);
    _ipv4Addr[addrLen] = '\0';

    strncpy(portStr, &_packedMsg[4 + addrLen + 1], portLen);
    portStr[portLen] = '\0';

    sscanf(portStr, "%d", _port);

    return PROTOCOL_SUCCESS;
}


PackedMessage ProtocolPackUserName(MSG_TYPE _type, char* _userName, size_t *_pckMsgSize) 
{
    PackedMessage packedMsg;
    Byte nameLen;

    if( _type >= MAX_MSG_TYPE || _userName == NULL)
    {
        return NULL;
    }

    nameLen = strlen(_userName);

    packedMsg = malloc( HEAD_BYTES * sizeof(Byte)  + nameLen * sizeof(char) + LENGTH_BYTES * sizeof(Byte) );
    if(packedMsg == NULL)
    {
        return NULL;
    }

    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + LENGTH_BYTES + nameLen);

    packedMsg[2] = nameLen;
    strncpy(&packedMsg[HEAD_BYTES + LENGTH_BYTES], _userName, nameLen);
    
    *_pckMsgSize = nameLen + HEAD_BYTES + LENGTH_BYTES; /* in Bytes! */
    return packedMsg;
}

PackedMessage ProtocolPackRespMsg(MSG_TYPE _type, MSG_RESPONSE _response, size_t *_pckMsgSize)
{
    PackedMessage packedMsg;

    if( _type >= MAX_MSG_TYPE || _pckMsgSize == NULL || _response >= MSG_RES_MAX)
    {
        return NULL;
    }

    packedMsg = malloc( (HEAD_BYTES + MSG_RSP_BYTES) * sizeof(Byte) );
    if(packedMsg == NULL)
    {
        return NULL;
    }

    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + MSG_RSP_BYTES);
    
    packedMsg[HEAD_BYTES] = (Byte)_response;

    *_pckMsgSize = HEAD_BYTES + MSG_RSP_BYTES; /* in Bytes! */

    return packedMsg;

}

PackedMessage ProtocolPackGroupName(MSG_TYPE _type, char* _groupName, size_t *_pckMsgSize) 
{
    PackedMessage packedMsg;
    Byte nameLen;

    if( _type >= MAX_MSG_TYPE || _groupName == NULL || _pckMsgSize == NULL)
    {
        return NULL;
    }

    nameLen = strlen(_groupName);

    packedMsg = malloc( HEAD_BYTES * sizeof(Byte)  + nameLen * sizeof(char) + LENGTH_BYTES * sizeof(Byte) );
    if(packedMsg == NULL)
    {
        return NULL;
    }

    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + LENGTH_BYTES + nameLen);

    packedMsg[2] = nameLen;
    strncpy(&packedMsg[HEAD_BYTES + LENGTH_BYTES], _groupName, nameLen);
    
    *_pckMsgSize = nameLen + HEAD_BYTES + LENGTH_BYTES; /* in Bytes! */
    return packedMsg;
}

PROTOCOL_ERR ProtocolUnpackGroupName(PackedMessage _packedMsg, char* _groupName)
{
    char nameLen;

    if(_packedMsg == NULL || _groupName == NULL)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }

    nameLen = _packedMsg[HEAD_BYTES];

    strncpy(_groupName, &_packedMsg[HEAD_BYTES + LENGTH_BYTES], nameLen);
    _groupName[nameLen] = '\0';

    return PROTOCOL_SUCCESS;
}


void ProtocolPackedMsgDestroy(PackedMessage _msg)
{
    if(_msg == NULL)
    {
        return;
    }
    free(_msg);
}

MSG_TYPE ProtocolGetMsgType(PackedMessage _packedMsg)
{
    if(_packedMsg == NULL)
    {
        return MSG_TYPE_ERR;
    }
    return (MSG_TYPE)_packedMsg[0];
}

PROTOCOL_ERR ProtocolUnpackUserDetails(PackedMessage _packedMsg, char* _userName, char* _userPass)
{
    char nameLen, passLen;

    if(_packedMsg == NULL || _userName == NULL || _userPass == NULL)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }

    nameLen = _packedMsg[2];
    passLen = _packedMsg[3 + nameLen];

    strncpy(_userName, &_packedMsg[3], nameLen);
    _userName[nameLen] = '\0';

    strncpy(_userPass, &_packedMsg[3 + nameLen + 1], passLen);
    _userPass[passLen] = '\0';

    return PROTOCOL_SUCCESS;
}

PROTOCOL_ERR ProtocolUnpackUserName(PackedMessage _packedMsg, char* _userName)
{
    if(_packedMsg == NULL || _userName == NULL)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }

    strncpy(_userName, _packedMsg + HEAD_BYTES + LENGTH_BYTES, _packedMsg[HEAD_BYTES]);
    _userName[_packedMsg[HEAD_BYTES]] = '\0';

    return PROTOCOL_SUCCESS;
}

MSG_RESPONSE ProtocolUnpackRespMsg(PackedMessage _packedMsg)
{
    if(_packedMsg == NULL)
    {
        return GEN_ERROR;
    }
    return (MSG_RESPONSE)(_packedMsg[HEAD_BYTES]);
}


PROTOCOL_ERR ProtocolCheck(PackedMessage _packMsg, size_t _rcvMsgSize)
{
    if(_packMsg == NULL || _rcvMsgSize < (1 + HEAD_BYTES))
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }

    if( (char)(_packMsg[1]) == (char)_rcvMsgSize )
    {
        return PROTOCOL_MSG_FULL;
    }
    else
    {
        return PROTOCOL_MSG_PART;
    }
}

PROTOCOL_ERR ProtocolEncrypt(PackedMessage _Msg, size_t _msgSize) /* TODO: Encrypt type? */ 
{
    size_t i;
    if(_Msg == NULL || _msgSize < 1)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }
    for(i = 0; i < _msgSize; i++)
    {
        /*(Byte)_Msg[i] = (Byte)_Msg[i];*/
    }
    return PROTOCOL_SUCCESS;
}

PROTOCOL_ERR ProtocolDecrypt(PackedMessage _Msg, size_t _msgSize)
{
    size_t i;
    if(_Msg == NULL || _msgSize < 1)
    {
        return PROTOCOL_MSG_NOT_INITALIZED;
    }
    for(i = 0; i < _msgSize; i++)
    {
        /*(char)_Msg[i] = ((char)_Msg[i] * 15 ) % MAX_CHAR_NUM;*/
    }
    return PROTOCOL_SUCCESS;
}


/* ------------------------- Helper Functions ------------------------- */
