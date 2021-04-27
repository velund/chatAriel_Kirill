


struct Client{
    socket;
} ;

Client* ClientNetConnect()

void ClientNetDisconnect()

ERR ClientNetSend(Client* _client, char* _packedMsg, size_t _pckMsgSize)
{
    send()
}