
#include <stdlib.h> /* size_t */
#include <string.h> /* strcmp */

#include "userMng.h"
#include "userStruct.h"
#include "HashMap.h"

#define MIN_HASH_SIZE 10

struct UserMng{
    HashMap* m_users;
} ;

/* ----------------------- HashMap Functions ----------------------- */

size_t UserMngHashFunc(void* _userName);
int UserMngHashEqualityFunc(void* _userName1, void* _userName2);
static void HashUserKeyDestroy(void *_userName);
static void HashUserDestroy(void *_userStruct);

/* ------------------- Helper Functions Prototypes ------------------- */



/* ------------------------- Main Functions ------------------------- */

UserMng* UserMngCreate(size_t _numOfBuckets)
{
    UserMng* newMng;

    if(_numOfBuckets < MIN_HASH_SIZE)
    {
        return NULL;
    }

    newMng = malloc(sizeof(sizeof(UserMng)));
    if(newMng == NULL)
    {
        return NULL;
    }

    newMng->m_users = HashMapCreate(_numOfBuckets, UserMngHashFunc, UserMngHashEqualityFunc);
    if(newMng->m_users == NULL)
    {
        return NULL;
    }


    return newMng;
}

void UserMngDestroy(UserMng** _userMng)
{
    if(_userMng == NULL || *_userMng == NULL)
    {
        return;
    }
    HashMapDestroy( &(*_userMng)->m_users, HashUserKeyDestroy, HashUserDestroy );
    free(*_userMng);
    *_userMng = NULL;
}

USER_MNG_ERR UserMngAdd(UserMng* _userMng, char* _name, char* _pass)
{
    User* newUser;
    char* newUserKey;
    MAP_RESULT hashRes;

    if(_userMng == NULL || _name == NULL || _pass == NULL)
    {
        return USER_MNG_NOT_INITALIZED;
    }

    newUserKey = malloc(strlen(_name) * sizeof(char));
    if(newUserKey == NULL)
    {
        return USER_MNG_CREATE_FAIL;
    }
    strcpy(newUserKey, _name);

    newUser = UserCreate(_name, _pass);
    if(newUser == NULL)
    {
        return USER_MNG_CREATE_FAIL;
    }

    hashRes = HashMapInsert(_userMng->m_users, (void*)newUserKey, (void*)newUser);

    if(hashRes == MAP_KEY_DUPLICATE_ERROR)
    {
        UserDestroy(&newUser);
        free(newUserKey);
        return USER_MNG_USER_EXISTS;
    }
    else if(hashRes == MAP_ALLOCATION_ERROR)
    {
        UserDestroy(&newUser);
        free(newUserKey);
        return USER_MNG_CREATE_FAIL;
    }
    else if(hashRes == MAP_SUCCESS)
    {
        return USER_MNG_USER_ADDED;
    }
}

USER_MNG_ERR UserMngRemove(UserMng* _userMng, char* _name)
{
    void* userNameOutput, *userStructOutPut;

    if(_userMng == NULL || _name == NULL)
    {
        return USER_MNG_NOT_INITALIZED;
    }

    if( HashMapRemove(_userMng->m_users, (void*)_name, &userNameOutput, &userStructOutPut) == MAP_KEY_NOT_FOUND_ERROR)
    {
        return USER_MNG_USER_NOT_FOUND;
    }

    free(userNameOutput);
    UserDestroy((User**)&userStructOutPut);

    return USER_MNG_SUCCESS;
}

USER_MNG_ERR UserMngConnect(UserMng* _userMng, char* _name, char* _pass)
{
    User *foundUser;
    USER_ERR reqRes;

    if(_userMng == NULL || _name == NULL || _pass == NULL)
    {
        return USER_MNG_NOT_INITALIZED;
    }
    if (HashMapFind(_userMng->m_users, (void*)_name, (void**)&foundUser) == MAP_KEY_NOT_FOUND_ERROR )
    {
        return USER_MNG_USER_NOT_FOUND;
    }

    reqRes = UserConnect(foundUser, _pass);

    switch (reqRes)
    {
    case USER_PASS_INCORRECT:
        return USER_MNG_PASS_INCORRECT;
        break;
    
    case USER_ALREADY_CONNECT:
        return USER_MNG_ALREADY_CONNECT;
        break;
    case USER_SUCCESS:
        return USER_MNG_SUCCESS;
        break;

    default:
        return USER_MNG_FAIL;
        break;
    }
    
}

USER_MNG_ERR UserMngDisconnect(UserMng* _userMng, char* _name)
{
    User *foundUser;

    if(_userMng == NULL || _name == NULL)
    {
        return USER_MNG_NOT_INITALIZED;
    }
    if (HashMapFind(_userMng->m_users, (void*)_name, (void**)&foundUser) == MAP_KEY_NOT_FOUND_ERROR )
    {
        return USER_MNG_USER_NOT_FOUND;
    }
    if (UserDisconnect(foundUser) != USER_SUCCESS)
    {
        return USER_MNG_DISCONNENT_ERR;
    }
    return USER_MNG_SUCCESS;
}



/* ------------------------- Hash Helper Functions ------------------------- */

size_t UserMngHashFunc(void* _userName) /* TODO: HASH key?? */
{
    size_t hashKey;

    if(_userName == NULL)
    {
        return 0;
    }
    
    hashKey = ((char*)_userName)[0];

    return hashKey;
}

int UserMngHashEqualityFunc(void* _userName1, void* _userName2)
{
    if(strcmp( (char*)_userName1, (char*)_userName2) == 0)
    {
        return 1;
    }
    return 0;
}

static void HashUserKeyDestroy(void *_userName)
{
    free(_userName);
}

static void HashUserDestroy(void *_userStruct)
{
    UserDestroy( (User**)&_userStruct );
}