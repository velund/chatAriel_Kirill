
ClientConnectToChat()
{
    ClientNetConnect();
}

ClientRegister(Client* _client, char* _name, char *_pass)
{
    
    size_t PackedMsgSize;
    Protocol(_name, _pass, &PackedMsgSize)
    ClientNetSend(Client* ,Protocol(), PackedMsgSize)
}