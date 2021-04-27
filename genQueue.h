/*****************************************************************************
* File Name: genQueue.h - Generic queue
* Written by: Ariel Kleiman
* Date: 18/3/21
* Credit to: Elior Nataf :)
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __GENQUEUE_H__
#define __GENQUEUE_H__

/*------------------------------- Header Files ------------------------------*/

#include <stdlib.h>		/*for size_t*/

/*------------------------------------ Enum  --------------------------------*/

typedef enum{
	QUEUE_SUCCESS,
	QUEUE_UNINITIALIZED_ERROR,
	QUEUE_OVERFLOW_ERROR,
	QUEUE_DATA_NOT_FOUND_ERROR,
	QUEUE_DATA_UNINITIALIZED_ERROR,
	QUEUE_IS_EMPTY
} QUEUE_ERR;

/*---------------------------------- Typedef --------------------------------*/

typedef struct Queue Queue;

/* Description:
 * Destroy function provided by the user to perform unique destory for for each element of the queue.
 * 
 * Time Complexity: O(1).
 *
 * Input:
 * element - pointer to element.
 */
typedef void (*DestroyItem)(void* _element);


/* Description:
 * Action function, provided by user, done for each element.
 * 
 * Time Complexity: O(1).
 *
 * Input:
 * element - pointer to element.
 * context - pointer to context if needed, NULL otherwise.
 *
 * Output:
 * 1 - to continue the action on each element.
 * 0 - to stop.
 *
 * Error: 
 * None.
 */
typedef int (*ActionFunction)(const void* _element, void* _context);

/*-------------------------- Functions declarations -------------------------*/


/**
 * @brief Create a new queue with a size of _size
 * @param[in] _size - The size of the queue
 * @return Queue pointer - on success, NULL on failure
 * @warning failure if given size 0 or failed to allocate memory
 */
Queue* QueueCreate(size_t _size);

/**
 * @brief Destroy a previously created queue
 * @param[in] _queue - the queue to Destroy
 * @param[in] _itemDestroy - Optional destrot function for item
 * @return void - no return value
 * @warning will not destroy if given Queue is not initalized.
 */
void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy);

/**
 * @brief Add a new item to the queue
 * @param[in] _queue - The queue to add the item to
 * @param[in] _item - the item to add to the queue
 * @return Queue_Result - 
 * @retval QUEUE_SUCCESS - on seccess
 * @retval QUEUE_UNINITIALIZED_ERROR - if _queue is NULL
 * @retval QUEUE_DATA_UNINITIALIZED_ERROR - if _item is NULL
 * @retval QUEUE_OVERFLOW_ERROR - if there is no more room to add 
 *
 */
QUEUE_ERR QueueInsert(Queue* _queue,void* _item);

/**
 * @brief Remove the first item in the queue 
 * @param[in] _queue - the queue to remove the information from
 * @param[in] _item - A pointer to the information removed
 * @return Queue_Result -
 * @retval QUEUE_SUCCESS - on seccess
 * @retval QUEUE_UNINITIALIZED_ERROR - if _queue is NULL
 * @retval QUEUE_DATA_UNINITIALIZED_ERROR - if _item is NULL
 * @retval QUEUE_DATA_NOT_FOUND_ERROR - if queue is empty
 *
 */
QUEUE_ERR QueueRemove(Queue* _queue,void** _item);

/**
 * @brief Check if a given queue is empty
 * @param[in] _queue - The queue to check
 * @return size_t - return none zero if empty
 *
 */
size_t QueueIsEmpty(Queue* _queue);

/**
 * @brief ForEach function to implement on all elements in queue
 * @param[in] _queue - The queue
 * @param[in] _action - The action to implement on elements
 * @param[in] _context - context for action
 * @return size_t - the number times action was implemented
 *
 * @details if action returns 0 stop running and return the number of times action run
 */
size_t QueueForEach(Queue* _queue, ActionFunction _action, void* _context);

#endif 		/*__GENQUEUE_H__*/
