/*****************************************************************************
* File Name: Chat user database
* Date: 25/4/21
*****************************************************************************/

/*****************************************************************************
* Description:
* 
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
 * Description: Create a user manager
 * Inputs: num of buckets ** TODO: ???
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
 * Outputs: USER_MNG_CLIENT_EXISTS, USER_MNG_CLIENT_ADDED
 * Errors: USER_MNG_NOT_INITALIZED, USER_MNG_ALLOC_FAIL, USER_MNG_CREATE_FAIL, 
*/
USER_MNG_ERR UserMngAdd(UserMng* _userMng, char* _name, char* _pass);

/*
 * Description: Remove a given client from user manager
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

/* TODO: UserMngIsConnect */ 

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
