/*****************************************************************************
* File Name: User struct ADT
* Date: 26/4/21
*****************************************************************************/

/*****************************************************************************
* Description:
* Generic user.
* Holds a name, password, connection flag and list of groups it connected currently.
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __USERSTRUCT_H__
#define __USERSTRUCT_H__

#define MIN_USERNAME_LENGTH 1
#define MIN_USER_PASS_LENGTH 1

#include "list.h"

/*------------------------------------ Enum  --------------------------------*/

typedef enum USER_ERR{
    USER_SUCCESS,
    USER_NOT_INITALIZED,
    USER_ALLOC_FAIL,

    USER_GRP_INSERT_FAIL,
    USER_GRP_NOT_FOUND,

    USER_NAME_SIMILAR,
    USER_NAME_NOT_SIMILAR,

    USER_PASS_CORRECT,
    USER_PASS_INCORRECT,

    USER_ALREADY_CONNECT,
    USER_GRP_LIST_FAIL,
    USER_IN_GRP,
    USER_NOT_IN__GRP
} USER_ERR;

/*---------------------------------- Typedef --------------------------------*/

typedef struct User User;

/*---------------------------------- Functions Declaretions --------------------------------*/

/*
 * Description: Create new user with given name and pass
 * Inputs: name and pass
 * Outputs: Pointer to user or NULL
 * Errors: NULL if allocation failed or user name or pass is shorter than #define above
*/
User* UserCreate(char* _name, char* _pass);

/*
 * Description: Destroy a given user
 * Inputs: pointer to user pointer
 * Outputs: NONE.
 * Errors: Won't destroy if given user not initalized or destory before.
*/
void UserDestroy(User** _user);

/*
 * Description: Check if given name and given user's name are the same
 * Inputs: pointer to name and to user.
 * Outputs: USER_NAME_SIMILAR, USER_NAME_NOT_SIMILAR
 * Errors: USER_NOT_VALID if given parameters not initalized, 
*/
USER_ERR UserNameCmp(char* _name, User* _user);

/*
 * Description: Check if given pass and given user's pass are the same
 * Inputs: pointer to pass and to user.
 * Outputs: USER_PASS_CORRECT, USER_PASS_INCORRECT
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, 
*/
USER_ERR UserPassCheck(char* _pass, User* _user);

/*
 * Description: Declare a given user as connected if pass correct, creates new list of group names the user connected to.
 * Inputs: User pointer and pass pointer.
 * Outputs: USER_PASS_INCORRECT, USER_SUCCESS
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, 
*/
USER_ERR UserConnect(User* _user, char* _pass); 

/*
 * Description: Declare a given user as disconnected, destroys the list of currently connected groups. @warning: get the list of groups prior to disconnection
 * Inputs: User pointer.
 * Outputs: USER_SUCCESS
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, 
*/
USER_ERR UserDisconnect(User* _user);


/*
 * Description: Get user's list of currently connected groups.
 * Inputs: User pointer, pointer to save list pointer to.
 * Outputs: USER_SUCCESS
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, 
*/
USER_ERR UserGetGrpList(User* _user, List** _grpList);


/*
 * Description: Declare user as joined the given group name, adds the name of the grp to user's list.
 * Inputs: User pointer, name of group
 * Outputs: USER_SUCCESS, USER_IN_GRP - if already in said group.
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, USER_ALLOC_FAIL, USER_GRP_INSERT_FAIL, 
*/
USER_ERR UserGroupJoined(User* _user, char* _grpName);


/*
 * Description: Declare user as left the given group name, removes the name of the group from user's list.
 * Inputs: User pointer, name of group
 * Outputs: USER_SUCCESS
 * Errors: USER_NOT_INITALIZED if given parameters not initalized, USER_GRP_NOT_FOUND
*/
USER_ERR UserGroupLeft(User* _user, char* _grpName);

/*
 * Description: Check if user connected to given group by group's name.
 * Inputs: User pointer, name of group
 * Outputs: USER_NOT_IN__GRP, USER_IN_GRP
 * Errors: USER_NOT_INITALIZED if given parameters not initalized
*/
USER_ERR UserIsConnectedToGrp(User* _user, char* _grpName);



#endif /* __USERSTRUCT_H__ */