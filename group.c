
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
    int m_socket;
    int m_port;
    char m_ipv4Address[IPV4_ADDRESS_SIZE];
} ;

/* ------------ HELPER FUNCTIONS PROTOTYPES ------------ */

static int GroupGetListenSocket(Group* _newGroup); /* returns INTERN_SUCCESS or INTERN_FAIL */
static void SetPortReusable(int _groupSocket);
static int InitGroupParameters(Group* _newGroup, char* _name, char* _ipv4Address, int _port);
static int InitGroupComm(Group* _newGroup);

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
    
    if(GroupGetListenSocket(newGroup) != INTERN_SUCCESS)
    {
        free(newGroup);
        return NULL;
    }
    
    SetPortReusable(newGroup->m_socket);

    if(InitGroupParameters(newGroup, _name, _ipv4Address, _port))
    {
        free(newGroup);
        return NULL;
    }
    
    if(InitGroupComm(newGroup))
    {
        free(newGroup);
        return NULL;
    }

    return newGroup;
}

void GroupDestroy(Group** _group)
{  
    if(_group == NULL || *_group == NULL)
    {
        return;
    }
    close((*_group)->m_socket);
    free(*_group);
}

int GroupConnect(Group* _group)
{
    
}



/* ------------------------------------- Helper FUNCTIONS ------------------------------------- */


static int GroupGetListenSocket(Group* _newGroup) /* returns INTERN_SUCCESS or INTERN_FAIL */
{
    _newGroup->m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(_newGroup->m_socket < 0)
    {
        perror("Group socket failed!");
        return INTERN_FAIL;
    }
    return INTERN_SUCCESS;
}

static void SetPortReusable(int _groupSocket)
{
    int reuseOptVal = 1;
    if(setsockopt(_groupSocket, SOL_SOCKET, SO_REUSEADDR, &reuseOptVal, sizeof(reuseOptVal)) < 0) /* setting as reusable  */
    {
        perror("Setting port as reusable failed!\n");
    }
}

static int InitGroupParameters(Group* _newGroup, char* _name, char* _ipv4Address, int _port)
{
    strcpy(_newGroup->m_name, _name);
    strcpy(_newGroup->m_ipv4Address, _ipv4Address);
    _newGroup->m_port = _port;
    return INTERN_SUCCESS;
}

static int InitGroupComm(Group* _newGroup)
{
    struct sockaddr_in mcast_group;
    struct ip_mreq mreq;

    memset(&mcast_group, 0, sizeof(mcast_group));

    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(_newGroup->m_port);
    mcast_group.sin_addr.s_addr = inet_addr(_newGroup->m_ipv4Address);

    if(bind(_newGroup->m_socket, (struct sockaddr*) &mcast_group, sizeof(mcast_group)) < 0)
    {
        perror("Group bind Failed! \n");
        return INTERN_FAIL;
    }

    mreq.imr_multiaddr = mcast_group.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(_newGroup->m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
	    perror ("add_membership setsockopt");
        return INTERN_FAIL;
	}

    return INTERN_SUCCESS;
}

