
#include <string.h> /* strlen, strcpy */
#include <stdlib.h> /* malloc, free */

#include "userStruct.h"
#include "list.h"
#include "list_itr.h"
#include "list_functions.h"


#define USER_NAME_SIZE 15
#define PASSWORD_SIZE 15

#define USER_ACTIVE 1
#define USER_INACTIVE 0

struct User {
    char m_name[USER_NAME_SIZE];
    char m_pass[PASSWORD_SIZE];
    int m_isActive;
    List* m_groups;
} ;

/* ------------------- Helper Functions Prototypes ------------------- */


static int GroupDisconnectAction(void* _group, void* _null);


/* ------------------------- Main Functions ------------------------- */

User* UserCreate(char* _name, char* _pass)
{
    User* newUser;

    if(_name == NULL || _pass == NULL || strlen(_name) < MIN_USERNAME_LENGTH || strlen(_pass) < MIN_USER_PASS_LENGTH)
    {
        return NULL;
    }

    newUser = malloc(sizeof(User));
    if(newUser == NULL)
    {
        return NULL;
    }

    newUser->m_groups = ListCreate();
    if(newUser->m_groups == NULL)
    {
        free(newUser);
        return NULL;
    }

    strcpy(newUser->m_name, _name);
    strcpy(newUser->m_pass, _pass);
    newUser->m_isActive = USER_INACTIVE;
    
    return newUser;
}

void UserDestroy(User** _user)
{
    if(_user == NULL || *_user == NULL)
    {
        return;
    }
    ListDestroy(&(*_user)->m_groups, NULL); 
    free(*_user);
    *_user = NULL;
}

USER_ERR UserNameCmp(char* _name, User* _user)
{
    if(_name == NULL || _user == NULL)
    {
        return USER_NOT_VALID;
    }
    if(strcmp(_name, _user->m_name) == 0)
    {
        return USER_NAME_SIMILAR;
    }
    return USER_NAME_NOT_SIMILAR;
}

USER_ERR UserPassCheck(char* _pass, User* _user)
{
    if(_pass == NULL || _user == NULL)
    {
        return USER_NOT_VALID;
    }
    if(strcmp(_pass, _user->m_pass) == 0)
    {
        return USER_PASS_CORRECT;
    }
    return USER_PASS_INCORRECT;
}

USER_ERR UserConnect(User* _user, char* _pass)
{
    if( _user == NULL )
    {
        return USER_NOT_INITALIZED;
    }
    if( strcmp(_user->m_pass, _pass) != 0 )
    {
        return USER_PASS_INCORRECT;
    }

    _user->m_isActive = USER_ACTIVE;
    return USER_SUCCESS;
}

USER_ERR UserDisconnect(User* _user)
{
    if( _user == NULL )
    {
        return USER_NOT_INITALIZED;
    }
    _user->m_isActive = USER_INACTIVE;
    return USER_SUCCESS;
}

USER_ERR UserGroupDisconnect(User* _user) /* TODO: Disconnect user from all groups */
{
    if(_user == NULL)
    {
        return USER_NOT_INITALIZED;
    }
    ListItrForEach(ListItrBegin(_user->m_groups), ListItrEnd(_user->m_groups), GroupDisconnectAction, NULL); /* TODO: group disconnect helper func */
    return USER_SUCCESS;
}

static int GroupDisconnectAction(void* _group, void* _null)
{

}