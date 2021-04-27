#ifndef __DOUBLELL_H__
#define __DOUBLELL_H__

#include <stdlib.h> /* size_t */
#include "ADTErr.h"
/* #include "../DoubleLLService.h" */
#define EMPTY 1
#define NOT_EMPTY 0
#define EXISTANCE 1098765
typedef struct List List;

/*A function pointer to be used to destroy elements inserted into the list*/
typedef void (*_elementDestroy)(void* _item);


/* 
  	description: Create a list, with head and tail sentinels (m_data in sewntinels = NULL)
	input: void
	output: reurns a pointer to the created list.
	err: NULL on failure due to allocation failure
 */
List* ListCreate(void);

/* 
  	description: Destroy list Destroys the list completely optionally destroys elements using user provided function
  	input: A pointer to previously created List returned via ListCreate *_pList will be null
	output: void
	err: void	
 */
void ListDestroy(List **_list, _elementDestroy _ed);

/*
	description: Add element to head of list O(1)
  	input: _list A previously created List ADT returned via ListCreat _item An item to add to the lis
	output: SUCCESS on success
	err: NULL_POINTER, MALLOC_FAIL
 */
ADTErr ListPushHead(List* _list, void *_data);

/*
	description: Add element to the tail of list O(1)
  	input: _list A previously created List ADT returned via ListCreat _item An item to add to the lis
	output: SUCCESS on success
	err: NULL_POINTER, MALLOC_FAIL
 */
ADTErr ListPushTail(List* _list, void *_data);

/* 	description:  Remove element from list's head. O(1)
    	if successfull, stores a pointer to the removed item in _item
  	input: A previously created List ADT returned via ListCreate. _pItem: To store a pointer to the removed item in
  	output: SUCCESS
  	err: NULL_POINTER
 */
ADTErr ListPopHead(List* _list, void **_data);

/* 	description:  Remove element from list's tail. O(1)
    	if successfull, stores a pointer to the removed item in _item
  	input: A previously created List ADT returned via ListCreate. _pItem: To store a pointer to the removed item in
  	output: SUCCESS
  	err: NULL_POINTER
 */
ADTErr ListPopTail(List* _list, void **_data);

/* 	description:  get number of items in list O(n)
  	input: A previously created List ADT returned via ListCreate.
  	output: SUCCESS
  	err: NULL_POINTER
 */
size_t ListSize( const List* _list);

/* 	description:  get number of items in list O(n)
  	input: A previously created List ADT returned via ListCreate.
  	output: 1 if list empty, 0 if not empty 
  	err: 1 if _list == null
 */
int ListIsEmpty(List* _list);
/* ListPrint is only for debug */
void        ListPrint(List* _list);

#endif /* __DOUBLELL_H__ */