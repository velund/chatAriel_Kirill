

void UIClientConnSuccess(ClientInfo _info);

void UIClientConnFail(ClientInfo _info);

void UIServerStart();

void UIMsgRcvErr();

void UIUserExistsRegErr(char* _name);

void UIUserCreated(char *_name);

void UIGroupCreated(char *_name, char* _ip, int _port);

void UIGroupJoined(int _clientID, char *_name, char* _ip, int _port);

void UIGroupLeft(int _clientID, char *_name);