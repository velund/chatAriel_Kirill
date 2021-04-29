
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* strlen(), memset() */
#include <netinet/in.h> /* IPPROTO_IP, sockaddr_in, htons() */
#include <stdio.h> /* perror() */
#include <errno.h> /* perror() */
#include <unistd.h> /* close() */
#include <arpa/inet.h> /* inet_addr */

#include "group.h"

#define MIN_PORT_NUM 1025
#define IPV4_ADDRESS_SIZE 20

#define INTERN_SUCCESS 0
#define INTERN_FAIL -1

struct Group{
    char m_name[MAX_GROUP_NAME_LENGTH];
    int m_port;
    char *m_ipv4Address;
    size_t m_numOfUsers;
} ;

/* ------------ HELPER FUNCTIONS PROTOTYPES ------------ */


/* ------------------------------------- Main FUNCTIONS ------------------------------------- */

Group *GroupCreate(char* _name, char* _ipv4Address, int _port)
{
    Group* newGroup;
    
    if(_name == NULL || strlen(_name) < MIN_GROUP_NAME_LENGTH || strlen(_name) > MAX_GROUP_NAME_LENGTH || _ipv4Address == NULL || _port < MIN_PORT_NUM)
    {
        return NULL;
    }
    
    newGroup = malloc(sizeof(Group));
    if(newGroup == NULL)
    {
        return NULL;
    }

    strcpy(newGroup->m_name, _name);
    newGroup->m_port = _port;
    /*strcpy(newGroup->m_ipv4Address, _ipv4Address);*/
    newGroup->m_ipv4Address = _ipv4Address;

    newGroup->m_numOfUsers = 1;
    return newGroup;
}

void GroupDestroy(Group** _group, char** _ipOutput)
{  
    if(_group == NULL || *_group == NULL)
    {
        return;
    }
    *_ipOutput = (*_group)->m_ipv4Address;
    free(*_group);
}

GROUP_ERR GroupConnect(Group* _group, char* _ipv4Address, int* _port)
{
    if(_group == NULL || _ipv4Address == NULL || _port == NULL)
    {
        return GROUP_NOT_INITALIZED;
    }

    strcpy(_ipv4Address, _group->m_ipv4Address);
    *_port = _group->m_port;

    (_group->m_numOfUsers)++;
    return GROUP_SUCCESS;
}

GROUP_ERR GroupDisconnect(Group* _group)
{
    if(_group == NULL)
    {
        return GROUP_NOT_INITALIZED;
    }

    (_group->m_numOfUsers)--;

    if(_group->m_numOfUsers == 0)
    {
        return GROUP_EMPTY;
    }
    return GROUP_SUCCESS;
}

GROUP_ERR GroupGetIP(Group* _group, char* _ipOutput)
{
    if(_group == NULL || _ipOutput == NULL)
    {
        return GROUP_NOT_INITALIZED;
    }
    strcpy(_ipOutput, _group->m_ipv4Address);
    return GROUP_SUCCESS;
}


GROUP_ERR GroupGetName(Group* _group, char* _nameOutput)
{

    if(_group == NULL || _nameOutput == NULL)
    {
        return GROUP_NOT_INITALIZED;
    }

    strcpy(_nameOutput, _group->m_name);

    return GROUP_SUCCESS;
}


/* ------------------------------------- Helper FUNCTIONS ------------------------------------- */
