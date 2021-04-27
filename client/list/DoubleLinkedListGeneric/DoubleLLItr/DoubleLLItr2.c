#include "DoubleLLItr2.h"
#include "../DoubleLLService.h" /* ListItr, predicateFunction, ListActionFunction */
typedef enum
{
	STOP,
	CONTINUE
}continueCondition;
typedef int (*userFunc)(void * _element, void* _context); 
typedef continueCondition (*loopManager)(int _status, void *_utility_context); 
typedef struct 
{
	userFunc m_userFunc;
	void * m_user_context;
	loopManager m_loopManagerFunc;
	void *m_utility_context;
}DataManagment;

/* assist funcs */
static DataManagment *initManager(userFunc _userFunc, void *_userContext);
static ListItr forAllElements(ListItr _start, ListItr _end, DataManagment *_manager);
static continueCondition findFirstManagerFunc(int _status, void *_utility_context);
static continueCondition CountIfManagerFunc(int _status, void *_utility_context);
static continueCondition fofEachManagerFunc(int _status, void *_utility_context);

/* end assist funcs  */

ListItr ListItr_FindFirst(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	Node *element;
	DataManagment *manager = initManager((userFunc)_predicate, _context);
	manager->m_loopManagerFunc = findFirstManagerFunc;
	element = forAllElements(_begin, _end, manager);
	if (manager != NULL){ free(manager); }
	return element;
	
}
size_t ListItr_CountIf(ListItr _begin, ListItr _end, PredicateFunction _predicate, void* _context)
{
	size_t counter=0;
	DataManagment *manager = initManager((userFunc)_predicate, _context);
	manager->m_loopManagerFunc = CountIfManagerFunc;
	manager->m_utility_context = (void*)(&counter);
	forAllElements(_begin, _end, manager);
	counter = *(size_t*)(manager->m_utility_context);
	if (manager != NULL){ free(manager); }
	return counter;
}
ListItr ListItr_ForEach(ListItr _begin, ListItr _end, ListActionFunction _action, void* _context)
{
	ListItr element = NULL;
	DataManagment *manager = initManager((userFunc)_action, _context);
	manager->m_loopManagerFunc = fofEachManagerFunc;
	manager->m_utility_context = (void*)element;
	element = forAllElements(_begin, _end, manager);
	if (manager != NULL){ free(manager); }
	if (((Node*)element)->m_prev == (Node*)element) { return element; }
	else
	return ((Node*)element)->m_prev;
}

/* assist funcs */
static DataManagment *initManager(userFunc _userFunc, void *_userContext)
{
	DataManagment *manager = (DataManagment*)malloc(sizeof(DataManagment));
	manager->m_userFunc = _userFunc;
	manager->m_user_context = _userContext;
	return manager;
}
static ListItr forAllElements(ListItr _start, ListItr _end, DataManagment *_manager)
{
	int status;
	continueCondition shouldContinue;
	Node *tmp = (Node*)_start;
	while (tmp != ((Node*)_end)->m_next)
	{
		status = (*(_manager->m_userFunc))(tmp->m_data, _manager->m_user_context);
		shouldContinue = (*(_manager->m_loopManagerFunc))(status, _manager->m_utility_context);
		if (!shouldContinue) { break; }
		tmp = tmp->m_next;
	}
   return tmp;
}
static continueCondition findFirstManagerFunc(int _status, void *_utility_context)
{
	if (_status != 0)
	{
		return STOP;
	}else
	{ return CONTINUE; }
	
}

static continueCondition CountIfManagerFunc(int _status, void *_utility_context)
{
	if (_status != 0) { *((size_t*)_utility_context) +=1; }
	return CONTINUE;
}
static continueCondition fofEachManagerFunc(int _status, void *_utility_context)
{
	if (_status == 0)
	{
		return STOP;
	}else
	return CONTINUE;
}



/* end assist funcs  */
