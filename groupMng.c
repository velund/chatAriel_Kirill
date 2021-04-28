
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* strcmp() */

#include "genQueue.h"
#include "HashMap.h"
#include "groupMng.h"
#include "group.h"

/*
        Note:
        Saves group in hash map using IP as key and group struct as value.
*/

#define HASH_MAP_SIZE 20
#define NUM_OF_MULTICAST_ADDR 5 /* TODO: how to increase string of IP?? */
#define IPV4_STRING_SIZE 20

#define PORT 5555

struct GroupMng{
    HashMap* m_groups;
    Queue *m_freeAddr;
} ;

typedef enum GROUP_MNG_ERR{
    GROUP_MNG_SUCCESS,
    GROUP_MNG_NOT_INITALIZED,
    
    GROUP_MNG_GET_ADDR_ERR,
}  GROUP_MNG_ERR;

/* -------------- Hash Map Functions Declaretions -------------- */

size_t GroupMngHashKeyFunc(void *_name);
int GroupMngHashEquality(void *_name1, void* _name2);

/* -------------- Main Helper Functions -------------- */

/* Create  */
static Queue* CreateFreeAddrQ();
static void DestroyAddrQ(Queue** _addrQ);
static char* CreateAddr(char* _addr);
static void DestroyAddrInQ(void* _addr);
static void IncrementIP(char* _ip); /* TODO: Increase String */

/* Destroy */
void GroupMngHashGroupKeyIPDestroy(void* _ip);
void GroupMngHashGroupValDestroy(void* _group);

/* ------------------------------------------------------ Main Functions ------------------------------------------------------ */

GroupMng* GroupMngCreate()
{
    GroupMng* newGroupMng;

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
    
    newGroupMng->m_freeAddr = CreateFreeAddrQ();
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
    HashMapDestroy( &(*_groupMng)->m_groups, GroupMngHashGroupKeyIPDestroy, GroupMngHashGroupValDestroy);
    DestroyAddrQ(&(*_groupMng)->m_freeAddr);
    free(*_groupMng);
    *_groupMng = NULL;
}

GROUP_MNG_ERR GroupMngAdd(GroupMng* _groupMng, char* _groupName)
{
    Group* newGroup;
    char *addr;

    if(_groupMng == NULL || _groupName == NULL)
    {
        return GROUP_MNG_NOT_INITALIZED;
    }

    if (QueueRemove(_groupMng->m_freeAddr, &addr) != QUEUE_SUCCESS)
    {
        return GROUP_MNG_GET_ADDR_ERR;
    }

    newGroup = GroupCreate(_groupName, addr, PORT );
    if(newGroup == NULL)
    {
        QueueInsert(_groupMng->m_freeAddr, (void*)addr);
    }

    
}



/* ------- Creation Helper ------- */

static Queue* CreateFreeAddrQ()
{
    size_t i;
    Queue* freeAddrQ;
    char addr[IPV4_STRING_SIZE] = "224.0.0.4";

    freeAddrQ = QueueCreate(NUM_OF_MULTICAST_ADDR);
    if(freeAddrQ == NULL)
    {
        return NULL;
    }

    for(i = 0; i < NUM_OF_MULTICAST_ADDR; i++)
    {
        if( QueueInsert(freeAddrQ, (void*)CreateAddr(addr)) != QUEUE_SUCCESS)
        {
            QueueDestroy(&freeAddrQ, DestroyAddrInQ);
            return NULL;
        }
        IncrementIP(addr);
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

static void IncrementIP(char* _ip) /* TODO: Increase String */
{
    size_t lastCharNum;

    lastCharNum = strlen(_ip);
    _ip[lastCharNum]++;
}

/* ------- Destroy Helper ------- */

void GroupMngHashGroupKeyIPDestroy(void* _ip)
{
    free(_ip);
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