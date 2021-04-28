
#include <stdlib.h> /* malloc(), free()*/
#include <string.h> /* strcmp(), strcat() */
#include <stdio.h> /* sprintf */

#include "genQueue.h"
#include "HashMap.h"
#include "groupMng.h"
#include "group.h"

/*
        Note:
        Saves group in hash map using name as key and group struct as value.
*/

#define HASH_MAP_SIZE 20
#define IPV4_STRING_SIZE 20
#define MIN_GROUP_NUM 1

#define PORT 5555

struct GroupMng{
    HashMap* m_groups;
    Queue *m_freeAddr;
} ;

/* -------------- Hash Map Functions Declaretions -------------- */

size_t GroupMngHashKeyFunc(void *_name);
int GroupMngHashEquality(void *_name1, void* _name2);

/* -------------- Main Helper Functions -------------- */

/* Create  */
static Queue* CreateFreeAddrQ(size_t _numOfAddress);
static void DestroyAddrQ(Queue** _addrQ);
static char* CreateAddr(char* _addr);
static void DestroyAddrInQ(void* _addr);

/* Destroy */
void GroupMngHashGroupKeyNameDestroy(void* _name);
void GroupMngHashGroupValDestroy(void* _group);

/* Add */
static char* HashNameAsKeyCreate(char* _name);

/* ------------------------------------------------------ Main Functions ------------------------------------------------------ */

GroupMng* GroupMngCreate(size_t _maxGroupsNum)
{
    GroupMng* newGroupMng;

    if(_maxGroupsNum < MIN_GROUP_NUM)
    {
        return NULL;
    }

    newGroupMng = malloc(sizeof(GroupMng));
    if(newGroupMng == NULL)
    {
        return NULL;
    }

    newGroupMng->m_groups = HashMapCreate(HASH_MAP_SIZE, GroupMngHashKeyFunc, GroupMngHashEquality);
    if(newGroupMng->m_groups == NULL)
    {
        free(newGroupMng);
        return NULL;
    }
    
    newGroupMng->m_freeAddr = CreateFreeAddrQ(_maxGroupsNum);
    if(newGroupMng->m_freeAddr == NULL)
    {
        HashMapDestroy(&newGroupMng->m_groups, NULL, NULL);
        free(newGroupMng);
        return NULL;
    }

    return newGroupMng;
}

void GroupMngDestroy(GroupMng** _groupMng)
{
    if(_groupMng == NULL || *_groupMng == NULL)
    {
        return;
    }
    HashMapDestroy( &(*_groupMng)->m_groups, GroupMngHashGroupKeyNameDestroy, GroupMngHashGroupValDestroy);
    DestroyAddrQ(&(*_groupMng)->m_freeAddr);
    free(*_groupMng);
    *_groupMng = NULL;
}

GROUP_MNG_ERR GroupMngAdd(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput)
{
    Group* newGroup;
    char *nameKey;
    char *freeAddr;

    if(_groupMng == NULL || _groupName == NULL || _ipOutput == NULL || _portOutput == NULL)
    {
        return GROUP_MNG_NOT_INITALIZED;
    }

    nameKey = HashNameAsKeyCreate(_groupName);
    if(nameKey == NULL)
    {
        return GROUP_MNG_ALLOC_ERR;
    }

    if (QueueRemove(_groupMng->m_freeAddr, &freeAddr) != QUEUE_SUCCESS)
    {
        free(nameKey);
        return GROUP_MNG_GET_ADDR_ERR;
    }

    newGroup = GroupCreate(_groupName, freeAddr, PORT );
    if(newGroup == NULL) /* returns address to queue */
    {
        QueueInsert(_groupMng->m_freeAddr, (void*)freeAddr);
        free(nameKey);
        return GROUP_MNG_CREATE_ERR;
    }

    if(HashMapInsert(_groupMng->m_groups, (void*)nameKey, (void*)newGroup ) != MAP_SUCCESS)
    {
        GroupDestroy(&newGroup);
        free(nameKey);
        QueueInsert(_groupMng->m_freeAddr, (void*)freeAddr);
        return GROUP_MNG_ADD_ERR;
    }

    strcpy(_ipOutput, freeAddr); /* return new group address */
    *_portOutput = PORT;
    return GROUP_MNG_SUCCESS;
}


GROUP_MNG_ERR GroupMngRemove(GroupMng* _groupMng, char* _groupName)
{
    Group* wantedGroup;
    char* groupKeyOutput;

    if(_groupMng == NULL || _groupName == NULL)
    {
        return GROUP_MNG_NOT_INITALIZED;
    }

    if( HashMapRemove(_groupMng->m_groups, (void*)_groupName, (void**)&groupKeyOutput, (void**)&wantedGroup ) != MAP_SUCCESS)
    {
        return GROUP_MNG_GROUP_NOT_EXISTS;
    }

    GroupDestroy(&wantedGroup);
    free(groupKeyOutput);
    return GROUP_MNG_SUCCESS;
}


GROUP_MNG_ERR GroupMngJoin(GroupMng* _groupMng, char* _groupName, char* _ipOutput, int* _portOutput)
{
    Group* wantedGroup;

    if(_groupMng == NULL || _groupName == NULL || _ipOutput == NULL || _portOutput == NULL)
    {
        return GROUP_MNG_NOT_INITALIZED;
    }

    if( HashMapFind(_groupMng->m_groups, (void*)_groupName, (void**)&wantedGroup) != MAP_SUCCESS)
    {
        return GROUP_MNG_GROUP_NOT_EXISTS;
    }

    if(GroupConnect(wantedGroup, _ipOutput, _portOutput) != GROUP_SUCCESS)
    {
        return GROUP_MNG_CONNECT_FAIL;
    }

    return GROUP_MNG_SUCCESS;
}

GROUP_MNG_ERR GroupMngLeave(GroupMng* _groupMng, char* _groupName)
{
    Group* wantedGroup;

    if(_groupMng == NULL)
    {
        return GROUP_MNG_NOT_INITALIZED;
    }

    if( HashMapFind(_groupMng->m_groups, (void*)_groupName, (void**)&wantedGroup) != MAP_SUCCESS)
    {
        return GROUP_MNG_GROUP_NOT_EXISTS;
    }

    if( GroupDisconnect(wantedGroup) != GROUP_SUCCESS)
    {
        return GROUP_MNG_DISCONNECT_FAIL;
    }

    return GROUP_MNG_SUCCESS;
}


/* ------- Creation Helper ------- */

static Queue* CreateFreeAddrQ(size_t _numOfAddress)
{
    size_t i;
    Queue* freeAddrQ;
    char addrBase[IPV4_STRING_SIZE] = "224.0.0.";
    char addrFinal[IPV4_STRING_SIZE];
    int addAddressInt;
    char addAddressCh[3];

    freeAddrQ = QueueCreate(_numOfAddress);
    if(freeAddrQ == NULL)
    {
        return NULL;
    }

    for(i = 0, addAddressInt = 1; i <= _numOfAddress; i++, addAddressInt++)
    {
        strcpy(addrFinal, addrBase);
        sprintf(addAddressCh, "%d", addAddressInt);
        strcat(addrFinal, addAddressCh);
        if( QueueInsert(freeAddrQ, (void*)CreateAddr(addrFinal)) != QUEUE_SUCCESS)
        {
            QueueDestroy(&freeAddrQ, DestroyAddrInQ);
            return NULL;
        }
    }
    return freeAddrQ;
}

static char* CreateAddr(char* _addr)
{
    char* newAddr;

    newAddr = malloc(sizeof(char) * IPV4_STRING_SIZE);
    if(newAddr == NULL)
    {
        return NULL;
    }

    strcpy(newAddr, _addr);
    return newAddr;
}

/* ------- Destroy Helper ------- */

void GroupMngHashGroupKeyNameDestroy(void* _name)
{
    free(_name);
}

void GroupMngHashGroupValDestroy(void* _group)
{
    GroupDestroy((Group**)&_group);
}

static void DestroyAddrInQ(void* _addr)
{
    if(_addr == NULL)
    {
        return;
    }
    free(_addr);
}

static void DestroyAddrQ(Queue** _addrQ)
{
    QueueDestroy(_addrQ, DestroyAddrInQ);
}

/* ------- Add Helper ------- */


static char* HashNameAsKeyCreate(char* _name)
{
    char* newkeyName;

    newkeyName = malloc(sizeof(char) * strlen(_name));
    if(newkeyName == NULL)
    {
        return NULL;
    }

    strcpy(newkeyName, _name);
    return newkeyName;
}

/* -------------- Hash Map Functions -------------- */

size_t GroupMngHashKeyFunc(void *_name)
{
    return (size_t)((char*)_name[0]);
}

int GroupMngHashEquality(void *_name1, void* _name2)
{
    if(strcmp( (char*)_name1, (char*)_name2) == 0 )
    {
        return 1;
    }
    return 0;
}