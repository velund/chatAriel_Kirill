
#ifndef __SERVERAPP_H__
#define __SERVERAPP_H__


#include "server.h"

typedef struct ServerApp ServerApp;

ServerApp* ServerAppCreate(ServerInfo _serInfo);

int ServerAppRun(ServerApp* _app); /* TODO: returns??? */

void ServerAppDestroy(ServerApp** _app);


#endif /* __SERVERAPP_H__ */