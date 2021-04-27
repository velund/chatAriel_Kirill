/*****************************************************************************
* File Name: Group struct ADT
* Date: 25/4/21
*****************************************************************************/

/*****************************************************************************
* Description:
* 
*****************************************************************************/

#define MAX_GROUP_NAME_LENGTH 15
#define MIN_GROUP_NAME_LENGTH 5

typedef struct Group Group;

typedef enum GROUP_ERR{
    GROUP_SUCCESS,
    GROUP_NOT_INITALIZED
    
} GROUP_ERR ;

Group *GroupCreate(char* _name);

void GroupDestroy(Group** _group);