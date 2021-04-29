/*****************************************************************************
* File Name: Group struct ADT
* Date: 25/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __GROUP_H__
#define __GROUP_H__

/*****************************************************************************
* Description:
* 
*****************************************************************************/

#define MAX_GROUP_NAME_LENGTH 20
#define MIN_GROUP_NAME_LENGTH 5

/*---------------------------------- Typedef --------------------------------*/

typedef struct Group Group;

/*---------------------------------- Enum --------------------------------*/

typedef enum GROUP_ERR{
    GROUP_SUCCESS,
    GROUP_NOT_INITALIZED,
    GROUP_EMPTY
} GROUP_ERR ;

/*-------------------------- Functions declarations -------------------------*/

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors: NULL
*/
Group *GroupCreate(char* _name, char* _ipv4Address, int _port);

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
void GroupDestroy(Group** _group, char** _ipOutput);

/*
 * Description: increment user number
 * Inputs:
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED
*/
GROUP_ERR GroupConnect(Group* _group, char* _ipv4Address, int* _port);

/*
 * Description: decrement user number
 * Inputs:
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED
*/
GROUP_ERR GroupDisconnect(Group* _group);

/*
 * Description: decrement user number
 * Inputs:
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED
*/
GROUP_ERR GroupGetIP(Group* _group, char* _ipOutput);


/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED
*/
GROUP_ERR GroupGetName(Group* _group, char* _nameOutput);


#endif /* __GROUP_H__ */ 