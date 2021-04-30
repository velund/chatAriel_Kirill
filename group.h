/*****************************************************************************
* File Name: Group struct ADT
* Date: 25/4/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __GROUP_H__
#define __GROUP_H__

/*****************************************************************************
* Description:
* Group Module
* Group has name, ipv4 address, port, counter of connections.
*****************************************************************************/

/*----------------------------- Editable Defines ----------------------------*/

#define MAX_GROUP_NAME_LENGTH 20
#define MIN_GROUP_NAME_LENGTH 5
#define MIN_PORT_NUM 1025

/*---------------------------------- Typedef --------------------------------*/

typedef struct Group Group;

/*---------------------------------- Enum --------------------------------*/

typedef enum GROUP_ERR{
    GROUP_SUCCESS,
    GROUP_NOT_INITALIZED,
    GROUP_EMPTY
} GROUP_ERR ;

/*-------------------------- Functions declarations -------------------------*/

/*
 * Description: Create new group with given IPv4 address and port. @warning: inserts given IP to group and copies name. DO NOT FREE IPV4 pointer
 * Inputs: Pointer to name string, IPv4 address string and port number.
 * Outputs: Pointer to created group or NULL
 * Errors: NULL if given parameters not initalized, given name too long or short (modified as a #define above), 
 *         port num too low (modified as a #define above), given IP address not valid for multicast.
*/
Group *GroupCreate(char* _name, char* _ipv4Address, int _port);

/*
 * Description: Destroy given group and extract the address.
 * Inputs: pointer to group pointer and pointer to save the extracted ip address.
 * Outputs:
 * Errors:
*/
void GroupDestroy(Group** _group, char** _ipOutput);

/*
 * Description: Connection request to group. outputs IPv4 address and port. *Counts the number of connection requests.
 * Inputs: Pointer to group, pointer to string to copy the IP address to, port num output.
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED if given parameters not initalized.
*/
GROUP_ERR GroupConnect(Group* _group, char* _ipv4Address, int* _port);

/*
 * Description: Disconnection request from the group. *Counts the number of disconnection requests.
 * Inputs: Pointer to group
 * Outputs: GROUP_SUCCESS, GROUP_EMPTY - if last connected client disconnected.
 * Errors: GROUP_NOT_INITALIZED if given parameters not initalized.
*/
GROUP_ERR GroupDisconnect(Group* _group);

/*
 * Description: Get ip address of group
 * Inputs: Pointer to group, pointer to string to copy the IP address to
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED if given parameters not initalized.
*/
GROUP_ERR GroupGetIP(Group* _group, char* _ipOutput);


/*
 * Description: Get Name of given group
 * Inputs: Pointer to group, pointer to string to copy the IP address to
 * Outputs: GROUP_SUCCESS
 * Errors: GROUP_NOT_INITALIZED if given parameters not initalized.
*/
GROUP_ERR GroupGetName(Group* _group, char* _nameOutput);


#endif /* __GROUP_H__ */ 