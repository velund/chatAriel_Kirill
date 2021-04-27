/*****************************************************************************
* File Name: Hash Map
* Written by: Ariel Kleiman
* Date: 05/04/21
*****************************************************************************/

/*------------------------------- Header Guard ------------------------------*/

#ifndef __HASHMAP_H__
#define __HASHMAP_H__

/*------------------------------- Header Files ------------------------------*/

#include <stddef.h>  /* size_t */

/*------------------------------- Description -------------------------------*/

/** 
 *  @file HashMap.h
 *  @brief Generic Hash map of key-value pairs implemented with separate chaining using linked lists.
 *	
 *  @details  The hash map (sometimes called dictionary or associative array)
 *  is a set of distinct keys (or indexes) mapped (or associated) to values.
 *  size of allocated table will be the nearest prime number greater than requestedcapacity
 *  Lists used for chaining will be allocated lazely.
 *
 */

/*---------------------------------- Typedef --------------------------------*/

typedef struct HashMap HashMap;

/*------------------------------------ Enum  --------------------------------*/

typedef enum MAP_RESULT {
	MAP_SUCCESS = 0,
	MAP_UNINITIALIZED_ERROR, 		/**< Uninitialized map error 	*/
	MAP_KEY_NULL_ERROR, 			/**< Key was null 				*/
	MAP_KEY_DUPLICATE_ERROR, 		/**< Duplicate key error 		*/
	MAP_KEY_NOT_FOUND_ERROR, 		/**< Key not found 				*/
	MAP_ALLOCATION_ERROR, 			/**< Allocation error 	 		*/
	MAP_GIVEN_PARAM_ERROR			/**< Parameters given NULL	 	*/
} MAP_RESULT;

/*-------------------------- User Functions typdef -------------------------*/

/* Description:
 * User Function that generates unique hash key for given data
 */
typedef size_t (*HashFunction)(void* _key);

/* Description:
 * User Function that check for equality of 2 given datas
 * returns 1 if equal or 0 if not
 */
typedef int (*EqualityFunction)(void* _firstKey, void* _secondKey);

/* Description:
 * User Function to operate an action on a given data in hash map
 * returns 1 if envoked or 0 if not
 */
typedef int	(*KeyValueActionFunction)(const void* _key, void* _value, void* _context);


/*-------------------------- Functions declarations -------------------------*/

/** 
 * @brief Create a new hash map with given capcity and key characteristics.
 * @param[in] _capacity - Expected max capacity 
 * 						  shall be rounded to nearest larger prime number.
 * @param[in] _hashFunc - hashing function for keys
 * @param[in] _keysEqualFunc - equality check function for keys. 
 * @return newly created map or null on failure
 * 
 * @retval NULL if given functions are not initalized OR given capacity is 0 or memory allocation fail.
 */
HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);


/**
 * @brief destroy hash map and set *_map to null
 * @param[in] _map : map to be destroyed
 * @param[optional] _keyDestroy : pointer to function to destroy keys
 * @param[optional] _valDestroy : pointer to function to destroy values 
 * @details optionally destroy all keys and values using user provided functions
 */
void HashMapDestroy(HashMap** _map, void (*_keyDestroy)(void* _key), void (*_valDestroy)(void* _value));


/** 
 * @brief Adjust map capacity and rehash all key/value pairs
 * @param[in] _map - exisiting map
 * @param[in] _newCapacity - new capacity shall be rounded to nearest larger prime number.
 * @return MAP_SUCCESS or MAP_ALLOCATION_ERROR or MAP_GIVEN_PARAM_ERROR (if given map is not initalized or capacity lower than 0)
 */
MAP_RESULT HashMapRehash(HashMap *_map, size_t newCapacity);


/** 
 * @brief Insert a key-value pair into the hash map.
 * @param[in] _map - Hash map adt to insert to, must be initialized
 * @param[in] _key - key to serve as index 
 * @param[in] _value - The value to associate with the key 
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_DUPLICATE_ERROR	if key already present in the map
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_ALLOCATION_ERROR on failer to allocate key-value pair
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and destinct
 */
MAP_RESULT HashMapInsert(HashMap* _map, const void* _key, const void* _value);


/** 
 * @brief Remove a key-value pair from the hash map.
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index to locate _value
 * @param[in] _pKey  - pointer to variable to recieve key from map if found
 * @param[in] _pValue - pointer to variable that will get the value if found.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and destinct
 */
MAP_RESULT HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue);


/** 
 * @brief Find a value by key
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index 
 * @param[in] _pValue - pointer to variable that will get the value if found.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR
 * 
 * @warning key must be unique and destinct
 */
MAP_RESULT HashMapFind(const HashMap* _map, const void* _key, void** _pValue);


/**
 * @brief Get number of key-value pairs inserted into the hash map
 */
size_t HashMapNumOfElements(const HashMap* _map);


/** 
 * This method is optional in the homewor 
 * @brief Iterate over all key-value pairs in the map.
 *                   Iteration will stop if _act returns a zero for a given pair
 * @details The user provided KeyValueActionFunction _action will be called for each element.  
 * @param[in] _map - Hash map to iterate over.
 * @param[in] _act - User provided function pointer to be invoked for each element
 * @returns number of times the user functions was invoked
 */
size_t HashMapForEach(const HashMap* _map, KeyValueActionFunction _action, void* _context);








/*-------------------------- Stats for Map -------------------------*/

#ifndef NDEBUG
#define NDEBUG

typedef struct MapStats {
	size_t numberOfBuckets;
	size_t numberOfChains;   
	size_t maxChainLength;
	size_t averageChainLength;
} MapStats;

MapStats HashMapGetStatistics(const HashMap* _map);

#endif /* NDEBUG */


#endif /* __HASHMAP_H__ */





