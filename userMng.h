/*****************************************************************************
* File Name: Chat user database
* Date: 25/4/21
*****************************************************************************/

/*****************************************************************************
* Description:
* User manager
* holds a list of users with usernames and passwords, groups connected to.
* Uses file name "savedUsers.txt" to hold user details. @warning: DO NOT DELETE "savedUsers.txt"
* TBD:
* 1. On creation - number of buckets is needed?
* 2. List of group every user is connected to - does actually needed?
* 3. Delete user details from file on remove.
* 4. Ask for user password on disconnection?
*****************************************************************************/

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



/* --------- SHOULD BE ACTUALLY USED ???? --------- */

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
USER_MNG_ERR UserMngGroupConnect();

/* 
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
USER_MNG_ERR UserMngGroupDisconnect();
