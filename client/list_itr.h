/*****************************************************************************
* File Name: List iterator
* By: Ariel
* Date: 22/3/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __LIST_ITR_H__
#define __LIST_ITR_H__

/*------------------------------- Header Files -----------------------------*/

#include "list.h"

/*--------------------------------- typedef --------------------------------*/

typedef void* ListItr;

/*-------------------------- Functions declarations -------------------------*/

/** 
 * @brief Get iterator to the list's beginning
 *
 * @params _list : list to return begin iterator, pointing at first element
 *                 or at the end if list is empty
 * @return iterator pointing at the list's beginning
 */
ListItr ListItrBegin(const List* _list);

/** 
 * @brief Get iterator to the list end
 *
 * @params _list : list to return end iterator
 * @return an iterator pointing at the list's end
 */
ListItr ListItrEnd(const List* _list);

/** 
 * @brief returns none zero if _a and _b iterators refer to same node
 */
int ListItrIsEquals(const ListItr _a, const ListItr _b);

/** 
 * @brief Get iterator to the next element from a given iterator
 * @warning if _itr is end _itr it will be returned
 */
ListItr ListItrNext(ListItr _itr);

/** 
 * @brief Get itertator to the previous element
 * @warning if _itr is begin _itr it will be returned
 */
ListItr ListItrPrev(ListItr _itr);

/** 
 * @brief Get data from the list node the iterator is pointing to
 *
 * @params _itr : A list iterator
 * @return the data the iterator is pointing at or NULL if pointing to the end
 */
void* ListItrGet(ListItr _itr);

/** 
 * @brief Set data at the node where the iterator is pointing at
 * @return the data from the node to be changed
 * @warning return NULL if given element is NULL (won't change the data in that case) or given listItr is NULL
 */
void* ListItrSet(ListItr _itr, void* _element);

/** 
 * @brief Inserts a new node before node the iterator is pointing at
 * @return an iterator pointing at the element inserted or NULL on error
 */
ListItr ListItrInsertBefore(ListItr _itr, void* _element);

/** 
 * @brief Removes the node the iterator is pointing at
 *
 * @params _itr : A list iterator
 * @return the removed data
 */
void* ListItrRemove(ListItr _itr);

#endif /* __LIST_ITR_H__ */