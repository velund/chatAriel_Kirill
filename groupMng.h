/*****************************************************************************
* File Name: Group Manager
* Date: 25/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __GROUPMNG_H__
#define __GROUPMNG_H__

/*****************************************************************************
* Description:
* Group Mananger
* Holds list of free IPv4 address for multicasting and list of current groups.
* 
* TBD:
* check if correct user left the group.
* i.e, user that actually joined the group has sent a leave request.
*****************************************************************************/

/*---------------------------------- Includes --------------------------------*/

#include "vector.h"

/*----------------------------- Editable Defines ----------------------------*/

#define MIN_GROUP_NUM 1
#define PORT 5555

/*---------------------------------- Typedef --------------------------------*/

typedef struct GroupMng GroupMng;

/*---------------------------------- Enum --------------------------------*/

typedef enum GROUP_MNG_ERR{
    GROUP_MNG_SUCCESS,
    GROUP_MNG_NOT_INITALIZED,
    GROUP_MNG_ALLOC_ERR,

    GROUP_MNG_OVERFLOW,
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
 * Description: Create new group manager
 * Inputs: max group number in a given moment.
 * Outputs: Group manager pointer or NULL
 * Errors: NULL if given max num is lower than min number defined above, or allocation failed.
*/
GroupMng* GroupMngCreate(size_t _maxGroupsNum);

/*
 * Description: Destroy a given group manager.
 * Inputs: Pointer to group manager pointer.
 * Outputs: NONE.
 * Errors: Won't destroy if given group manager not initalized.
*/
void GroupMngDestroy(GroupMng** _groupMng);

/*
 * Description: Add group in given group mananger. Name should be unique!
 * Inputs: Group manager pointer, name of the group, and outputs for IP and port.
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_ALLOC_ERR, GROUP_MNG_GET_ADDR_ERR, GROUP_MNG_CREATE_ERR, GROUP_MNG_ADD_ERR
*/
GROUP_MNG_ERR GroupMngAdd(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput);

/*
 * Description: Remove a group in given group manager with it's unique name.
 * Inputs: Group manager pointer, name of the group.
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED,GROUP_MNG_GROUP_NOT_EXISTS
*/
GROUP_MNG_ERR GroupMngRemove(GroupMng* _groupMng, char* _groupName);

/*
 * Description: Join a group in given group manager. @warning: every join should be followed by a leave function.
 * Inputs: Group manager pointer, name of the group, and outputs for IP and port.
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_GROUP_NOT_EXISTS, GROUP_MNG_CONNECT_FAIL
*/
GROUP_MNG_ERR GroupMngJoin(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput);

/*
 * Description: Leave a group in given group manager. If no users left in the group, group will be destroyed.
 * Inputs: Group manager pointer, name of the group.
 * Outputs: GROUP_MNG_SUCCESS
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_GROUP_NOT_EXISTS, GROUP_MNG_DISCONNECT_FAIL
*/
GROUP_MNG_ERR GroupMngLeave(GroupMng* _groupMng, char* _groupName);

/*
 * Description: Get the list of group in group manager inserted into a given initalized vector.
 * Inputs: Group manager pointer, vector pointer. @warning: Vector should be initalized beforehand.
 * Outputs: GROUP_MNG_SUCCESS, GROUP_MNG_EMPTY - if no active groups
 * Errors: GROUP_MNG_NOT_INITALIZED, GROUP_MNG_GET_GROUPS_ERR if error occurred while names inserted into the vector.
*/
GROUP_MNG_ERR GroupMngGetGroupList(GroupMng* _groupMng, Vector *_list);


#endif /* __GROUPMNG_H__ */