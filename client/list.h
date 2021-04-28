/*****************************************************************************
* File Name: Generic Double linked List
* By: Ariel
* Date: 21/3/21
*****************************************************************************/

/** 
 *  @file list.h
 *  @brief Create a Generic Double Linked List data type
 * 
 *  @details Implemented as double linked list using head and tail sentinels.
 *  
 *  @author Ariel
 * 
 */ 

/*------------------------------- Header Guard ------------------------------*/

#ifndef __LIST_H__
#define __LIST_H__

/*------------------------------- Header Files -----------------------------*/
#include <stddef.h>  /* size_t */

/*---------------------------------- Typedef -------------------------------*/

typedef struct List List;

/*---------------------------------- enum ----------------------------------*/

typedef enum LIST_ERR {
	LIST_SUCCESS,
	LIST_UNINITIALIZED_ERROR,			/**< Uninitialized list 					 	*/
	LIST_ALLOCATION_ERROR,				/**< Node allocation failed due to heap error   */
	LIST_NULL_ELEMENT_ERROR,
	LIST_EMPTY
	/* Add more as needed by your implementation */
	
} LIST_ERR;

/*-------------------------- Functions declarations -------------------------*/

/** 
 * @brief Create a double linked list
 * 
 * @returns a pointer to the created list.
 * @retval NULL on failure due to allocation failure
 */
List* ListCreate(void);

/** 
 * @brief Destroy list
 * @details Destroys the list completely 
 *          optionally destroys elements using user provided function
 *
 * @params[in] _pList : A pointer to previously created List returned via ListCreate
 *					   on completion *_pList will be null
 * @params[in] _elementDestroy : A function pointer to be used to destroy elements inserted into the list
 *             or a NULL if no such destroy is required
 */
void ListDestroy(List** _pList, void (*_elementDestroy)(void* _item));

/** 
 * @brief Add element to head of list
 * @details time complexity: O(1).
 *
 * @params _list A previously created List ADT returned via ListCreate
 * @params _item An item to add to the list
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_ALLOCATION_ERROR on memory allocation failure
 */
LIST_ERR ListPushHead(List* _list, void* _item);

/** 
 * @brief Add element to the list's tail
 * @details time complexity O(1).
 *
 * @params _list A previously created List ADT returned via ListCreate
 * @params _item An item to add to the list
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_ALLOCATION_ERROR on memory allocation failure
 */
LIST_ERR ListPushTail(List* _list, void* _item);

/** @brief Remove element from list's head
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store the pointer of the removed item
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_EMPTY if no items in the list
 */
LIST_ERR ListPopHead(List* _list, void** _pItem);

/** @brief Remove element from list's tail
 *  @details time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store the pointer of the removed item
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_EMPTY if no items in the list
 */
LIST_ERR ListPopTail(List* _list, void** _pItem);

/** @brief Get number of elements in the list
 * Average time complexity O(n).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 */
size_t ListSize(const List* _list);


#endif /* __LIST_H__ */
