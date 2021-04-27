/*****************************************************************************
* File Name: User struct ADT
* Date: 26/4/21
*****************************************************************************/

/*****************************************************************************
* Description:
* 
*****************************************************************************/

#define MIN_USERNAME_LENGTH 1
#define MIN_USER_PASS_LENGTH 1

/*------------------------------------ Enum  --------------------------------*/

typedef enum USER_ERR{
    USER_SUCCESS,
    USER_NOT_INITALIZED,

    USER_NOT_VALID,

    USER_NAME_SIMILAR,
    USER_NAME_NOT_SIMILAR,

    USER_PASS_CORRECT,
    USER_PASS_INCORRECT
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
 * Errors: USER_NOT_VALID if given parameters not initalized, 
*/
USER_ERR UserPassCheck(char* _pass, User* _user);

/*
 * Description: Declare a given user as connected if pass correct.
 * Inputs: User pointer and pass pointer.
 * Outputs: USER_PASS_INCORRECT, USER_SUCCESS
 * Errors: USER_NOT_VALID if given parameters not initalized, 
*/
USER_ERR UserConnect(User* _user, char* _pass); 

/*
 * Description: Declare a given user as disconnected.
 * Inputs: User pointer.
 * Outputs: USER_SUCCESS
 * Errors: USER_NOT_VALID if given parameters not initalized, 
*/
USER_ERR UserDisconnect(User* _user);

/*
 * Description: 
 * Inputs:
 * Outputs:
 * Errors:
*/
USER_ERR UserGroupDisconnect(User* _user); /* TODO: Disconnect user from all groups */ 