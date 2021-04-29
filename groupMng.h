/*****************************************************************************
* File Name: Group Manager
* Date: 25/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __GROUPMNG_H__
#define __GROUPMNG_H__

/*****************************************************************************
* Description:
* 
*****************************************************************************/

#include "vector.h"

/*---------------------------------- Typedef --------------------------------*/

typedef struct GroupMng GroupMng;

/*---------------------------------- Enum --------------------------------*/

typedef enum GROUP_MNG_ERR{
    GROUP_MNG_SUCCESS,
    GROUP_MNG_NOT_INITALIZED,
    GROUP_MNG_ALLOC_ERR,
    
    GROUP_MNG_GET_ADDR_ERR,
    GROUP_MNG_CREATE_ERR,
    GROUP_MNG_ADD_ERR,
    GROUP_MNG_CONNECT_FAIL,
    GROUP_MNG_DISCONNECT_FAIL,

    GROUP_MNG_GROUP_NOT_EXISTS,
    GROUP_MNG_EMPTY,

    GROUP_MNG_GET_GROUPS_ERR
    
}  GROUP_MNG_ERR;

/*-------------------------- Functions declarations -------------------------*/

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors: NULL
*/
GroupMng* GroupMngCreate(size_t _maxGroupsNum);

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
void GroupMngDestroy(GroupMng** _groupMng);

/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_ALLOC_ERR, GROUP_MNG_GET_ADDR_ERR, GROUP_MNG_CREATE_ERR, GROUP_MNG_ADD_ERR
*/
GROUP_MNG_ERR GroupMngAdd(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput);

/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED,GROUP_MNG_GROUP_NOT_EXISTS
*/
GROUP_MNG_ERR GroupMngRemove(GroupMng* _groupMng, char* _groupName);

/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_GROUP_NOT_EXISTS, GROUP_MNG_CONNECT_FAIL
*/
GROUP_MNG_ERR GroupMngJoin(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput);

/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_GROUP_NOT_EXISTS, GROUP_MNG_DISCONNECT_FAIL
*/
GROUP_MNG_ERR GroupMngLeave(GroupMng* _groupMng, char* _groupName);


/*
 * Description: 
 * Inputs:
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_GET_GROUPS_ERR, GROUP_MNG_NOT_INITALIZED
*/
GROUP_MNG_ERR GroupMngGetGroupList(GroupMng* _groupMng, Vector *_list);


#endif /* __GROUPMNG_H__ */