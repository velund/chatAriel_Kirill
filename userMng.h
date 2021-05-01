/*****************************************************************************
* File Name: Chat user database
* Date: 25/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __USERMNG_H__
#define __USERMNG_H__

/*****************************************************************************
* Description:
* User manager
* holds a list of users with usernames and passwords, groups connected to.
* Uses file name "savedUsers.txt" to hold user details. @warning: DO NOT DELETE "savedUsers.txt"
* TBD:
* 1. On creation - number of buckets is needed?
* 2. Delete user details from file on remove.
* 3. Ask for user password on disconnection?
*****************************************************************************/

#include "list.h"

/*---------------------------------- Typedef --------------------------------*/

typedef struct UserMng UserMng;

/*---------------------------------- ENUM --------------------------------*/

typedef enum USER_MNG_ERR{
    USER_MNG_SUCCESS,
    USER_MNG_NOT_INITALIZED,

    USER_MNG_ALLOC_FAIL,
    USER_MNG_CREATE_FAIL,
    USER_MNG_USER_EXISTS,
    USER_MNG_USER_ADDED,

    USER_MNG_USER_NOT_FOUND,
    USER_MNG_PASS_INCORRECT,
    USER_MNG_ALREADY_CONNECT,

    USER_MNG_DISCONNENT_ERR,

    USER_MNG_GRP_JOIN_FAIL,
    USER_MNG_GRP_LEAVE_FAIL,
    USER_MNG_IN_GRP,
    USER_MNG_NOT_IN_GRP,

    USER_MNG_FAIL
} USER_MNG_ERR ;


/*---------------------------------- Functions Declaretions --------------------------------*/

/*
 * Description: Create a user manager, reads existing users from file "savedUsers.txt"
 * Inputs: num of buckets
 * Outputs: Pointer to user manager or NULL
 * Errors: NULL if allocation failed or  _numOfBuckets < 10
*/
UserMng* UserMngCreate(size_t _numOfBuckets);

/*
 * Description: Destroy a given user manager with all it's content
 * Inputs: Pointer to user mananger pointer.
 * Outputs: NONE.
 * Errors: Won't destroy if given mananger not initalized or previously destroyed.
*/
void UserMngDestroy(UserMng** _userMng);

/*
 * Description: Add new client to user mananger.
 * Inputs: User manager pointer, name and pass
 * Outputs: USER_MNG_CLIENT_EXISTS - if given username is taken, USER_MNG_CLIENT_ADDED
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_ALLOC_FAIL, USER_MNG_CREATE_FAIL, 
*/
USER_MNG_ERR UserMngAdd(UserMng* _userMng, char* _name, char* _pass);

/*
 * Description: Remove a given client from user manager. 
 * Inputs: User manager pointer and user's name
 * Outputs: USER_MNG_USER_NOT_FOUND, USER_MNG_SUCCESS
 * Errors: USER_MNG_NOT_INITALIZED, 
*/
USER_MNG_ERR UserMngRemove(UserMng* _userMng, char* _name);

/*
 * Description: Connect to given user in user mananger
 * Inputs: User manager pointer, name and pass
 * Outputs: USER_MNG_USER_NOT_FOUND, USER_MNG_PASS_INCORRECT, USER_MNG_SUCCESS
 * Errors:USER_MNG_NOT_INITALIZED, 
*/
USER_MNG_ERR UserMngConnect(UserMng* _userMng, char* _name, char* _pass);

/*
 * Description: Disconnect a given user from user manager
 * Inputs: User manager pointer and user's name
 * Outputs: USER_MNG_USER_NOT_FOUND, USER_MNG_SUCCESS
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_DISCONNENT_ERR
*/
USER_MNG_ERR UserMngDisconnect(UserMng* _userMng, char* _name);

/*
 * Description: Declare given user as connected to given group
 * Inputs: User manager pointer, user's name, and group name
 * Outputs: USER_MNG_SUCCESS, USER_MNG_IN_GRP - if user already in grp
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_USER_NOT_FOUND, USER_MNG_GRP_JOIN_FAIL
*/
USER_MNG_ERR UserMngGroupJoined(UserMng* _userMng, char* _userName, char* _grpName);

/* 
 * Description: Declare given user as disconnected from given group
 * Inputs: User manager pointer, user's name, and group name
 * Outputs: USER_MNG_SUCCESS
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_USER_NOT_FOUND, USER_MNG_GRP_LEAVE_FAIL
*/
USER_MNG_ERR UserMngGroupLeft(UserMng* _userMng, char* _userName, char* _grpName);

/* 
 * Description: Check if given user connected to given group.
 * Inputs: User manager pointer, user's name, and group name
 * Outputs: USER_MNG_NOT_IN_GRP, USER_MNG_IN_GRP
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_USER_NOT_FOUND, USER_MNG_GRP_LEAVE_FAIL
*/
USER_MNG_ERR UserMngIsUserInGrp(UserMng* _userMng, char* _userName, char* _grpName);

/* TODO: should copy or be given?
 * Description: Get the list of groups names the client currently connected to
 * Inputs: User manager pointer, user's name, pointer to save the list pointer to.
 * Outputs: USER_MNG_SUCCESS
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_USER_NOT_FOUND, USER_MNG_FAIL - if failed extracting the list from given user
*/
USER_MNG_ERR UserMngGetUserGrps(UserMng* _userMng, char* _userName, List** _grpList);


#endif /* __USERMNG_H__ */