
#include <stdio.h>
#include <stdlib.h>

#include "clientUI.h"
#include "protocol.h"

CLIENT_ACT ClientUIStartMenu()
{
    int userAct = 0;
    /*system("clear");*/
    while(userAct != 1 && userAct != 2 && userAct != 3)
    {
        
        printf("\n ------ Main Menu ------\n\n");
        printf("Choose your action: \n");
        printf("1: Register\n2: Login\n3: Exit\n\n");
        printf("Your choice: ");
        scanf("%d", &userAct);
        fflush(stdin);
        switch (userAct)
        {
        case 1:
            return ACT_REG;
            break;
        case 2:
            return ACT_LOG;
            break;
        case 3:
            return ACT_EXIT;
            break;
        default:
            printf("Please enter a valid action! \n");
            break;
        }
        
    }
    putchar('\n');
    
}

CLIENT_ACT ClientUIGetUserDetails(char* _userName, char* _passWord)
{
    if(_userName == NULL || _passWord == NULL)
    {
        return ACT_ERR;
    }

    printf("Please enter username: ");
    scanf("%s", _userName);
    fflush(stdin);

    printf("Please enter password: ");
    scanf("%s", _passWord);
    fflush(stdin);
    putchar('\n');
    return ACT_SUCCESS;
}

void ClientUIErr(MSG_RESPONSE _err)
{
    switch (_err)
    {
    /* Register */
    case USER_EXISTS:
        printf("User exists!\n");
        break;
    case USER_NAME_TOO_SHORT:
        printf("Username too short!\n");
        break;
    case PASS_TOO_SHORT:
        printf("Password too short!\n");
        break;

    /* Login */
    case PASS_INCORRECT:
        printf("Incorrect password!\n");
        break;
    case USER_NOT_FOUND:
        printf("Username not found!\n");
        break;

    /* Groups */
    case GROUP_CREATE_FAIL:
        printf("Group creation failed!\n");
        break;
    case GROUP_JOIN_FAIL:
        printf("Joining group failed!\n");
        break;
    default:
        printf("Unknown Err %d\n", _err);
        break;
    }
}


CLIENT_ACT ClientUIMenu2()
{
    int userAct = 0;

    while(userAct != 1 && userAct != 2 && userAct != 3 && userAct != 4)
    {
        system("clear");
        printf("\n\n ------ Menu 2 ------\n\n");

        printf("Choose your action: \n");
        printf("1: Create group\n");
        printf("2: Join group\n");
        printf("3: Leave group\n");
        printf("4: Logout\n\n");
        printf("Your choice: ");
        scanf("%d", &userAct);
        fflush(stdin);

        switch (userAct)
        {
        case 1:
            return ACT_CREATE_GRP;
            break;
        case 2:
            return ACT_JOIN_GRP;
            break;
        case 3:
            return ACT_LEAVE_GRP;
            break;
        case 4:
            return ACT_LOGOUT;
            break;
        default:
            printf("Please enter a valid action! \n");
            break;
        }
    }
}

CLIENT_ACT ClientUIGetGroupName(char* _groupName)
{
    if(_groupName == NULL)
    {
        return ACT_ERR;
    }

    printf("Please enter group name: ");
    scanf("%s", _groupName);

    putchar('\n');
    return ACT_SUCCESS;
}


int showGroup(void* _element, size_t _index, void* _context)
{
    if ( (char*)_element == NULL ) { return 0; }
    printf("%ld: %s\n", _index + 1, (char*)_element );
    return 1;
}

void GroupNameInVecDestroy(void *_name)
{
    free(_name);
}

void showGroups(Vector *_groups)
{
    VectorForEach(_groups, showGroup, NULL);
    VectorDestroy(&_groups, GroupNameInVecDestroy);
}
