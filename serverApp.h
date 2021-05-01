/*****************************************************************************
* File Name: Server App
* Date: 26/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __SERVERAPP_H__
#define __SERVERAPP_H__


/*---------------------------------- Includes --------------------------------*/

#include "server.h"

/*---------------------------------- Typedef --------------------------------*/

typedef struct ServerApp ServerApp;


/*-------------------------- Functions declarations -------------------------*/

/*
 * Description: Create new server app for chat with given server info
 * Inputs: ServerInfo
 * Outputs: Server app pointer or NULL
 * Errors: 
*/
ServerApp* ServerAppCreate(ServerInfo _serInfo);

/*
 * Description: 
 * Inputs: 
 * Outputs: 
 * Errors: 
*/
int ServerAppRun(ServerApp* _app); /* TODO: returns??? */

/*
 * Description: 
 * Inputs: 
 * Outputs: 
 * Errors: 
*/
void ServerAppDestroy(ServerApp** _app);


#endif /* __SERVERAPP_H__ */