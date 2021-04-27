
#include <stdlib.h> /* malloc() */
#include <string.h> /* memcpy(), strlen() */
#include <arpa/inet.h> /*  */

#include "protocol.h"


/*
    @WARNING:
        Packed message length parameter is original message size + header size + lengths!

    
    UserDetailsPackedMsg =
    [MSG_TYPE][Whole Msg Length][UserNameLength][UserName(No '\0')][PassLength][Pass(No '\0')]

    UserNamePackMsg = 
    [MSG_TYPE][Whole Msg Length][UserNameLength][UserName(No '\0')]

    ResponsePackMsg = 
    [MSG_TYPE][Whole Msg Length][MSG_RESPONSE]
*/

typedef char Byte;

#define HEAD_BYTES 2 
#define LENGTH_BYTES 1
#define MSG_RSP_BYTES 1

#define MAX_CHAR_NUM 255



/* ------------------------- Helper Functions Prototypes ------------------------- */

static void InitPackHeader(PackedMessage _pckMsg, MSG_TYPE _type, size_t _msgSize);


/* ------------------------- Main Functions ------------------------- */

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

    /*InitPackHeader(packedMsg, _type, HEAD_BYTES + nameLen + passLen + 2 * LENGTH_BYTES);*/
    packedMsg[0] = (Byte)_type;
    packedMsg[1] = (Byte)( HEAD_BYTES + nameLen + passLen + 2);


    packedMsg[2] = nameLen;
    strncpy(&packedMsg[3], _userName, nameLen);

    packedMsg[3 + nameLen] = passLen;
    strncpy(&packedMsg[3 + nameLen + 1], _userPass, passLen);

    *_pckMsgSize = 2 + nameLen + passLen + 2; /* in Bytes! */

    return packedMsg;
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

    InitPackHeader(packedMsg, _type, HEAD_BYTES + nameLen + LENGTH_BYTES);

    packedMsg[HEAD_BYTES] = nameLen;
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

    InitPackHeader(packedMsg, _type, HEAD_BYTES + MSG_RSP_BYTES);
    packedMsg[HEAD_BYTES] = (Byte)_response;

    *_pckMsgSize = HEAD_BYTES + MSG_RSP_BYTES; /* in Bytes! */

    return packedMsg;

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

static void InitPackHeader(PackedMessage _pckMsg, MSG_TYPE _type, size_t _msgSize)
{
    _pckMsg[0] = (Byte)_type;
    _pckMsg[1] = (Byte)( _msgSize);
}