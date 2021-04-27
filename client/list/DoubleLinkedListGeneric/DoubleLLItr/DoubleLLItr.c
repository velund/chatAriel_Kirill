#include <stdlib.h> /* NULL */
#include "DoubleLLItr.h"
#include "../DoubleLLService.h" /* ListItr, predicateFunction, ListActionFunction */
ListItr ListItrBegin(const List* _list)
{	
	if (_list == NULL) { return NULL; }
	return (ListItr)_list->m_head.m_next;
} 
ListItr ListItrEnd(const List* _list)
{
	if (_list == NULL) { return NULL; }
	return (ListItr)_list->m_tail.m_next;
}
int ListItrEquals(const ListItr _a, const ListItr _b)
{
	if (_a == NULL || _b == NULL) { return 0;}
	if ((Node*)_a == (Node*)_b) { return 1; }
	return 0;
}
ListItr ListItrNext(ListItr _itr)
{
	if (_itr == NULL) { return NULL; }
	return (ListItr)(((Node*)_itr)->m_next);
}
ListItr ListItrPrev(ListItr _itr)
{
	if (_itr == NULL) { return NULL; }
	if ( ((Node*)_itr)->m_prev->m_prev ==  ((Node*)_itr)->m_prev )
	{ return _itr; }
	return (ListItr)(((Node*)_itr)->m_prev);
}
void* ListItrGet(ListItr _itr)
{
	if (_itr == NULL) { return NULL; }
	if ( ((Node*)_itr)->m_next ==  (Node*)_itr) { return NULL; }
	return (void*)(((Node*)_itr)->m_data);
}
void* ListItrSet(ListItr _itr, void* _element)
{
	void* tmp;
	if (_itr == NULL || _element == NULL) { return NULL; }
	if ( ((Node*)_itr)->m_next == (Node*)_itr ) { return NULL; }
	tmp = ((Node*)_itr)->m_data;
	((Node*)_itr)->m_data = _element;
	return tmp;
}
ListItr ListItrInsertBefore(ListItr _itr, void* _element)
{
	Node *newNode;
	if (_itr == NULL || _element == NULL) {return NULL;}
	if ( (newNode = malloc(sizeof(Node))) == NULL) {return NULL;}
	newNode->m_data = _element;
	newNode->m_next = (Node*)_itr;
	newNode->m_prev = ((Node*)_itr)->m_prev;
	((Node*)_itr)->m_prev->m_next = newNode;
	((Node*)_itr)->m_prev = newNode;
	return (ListItr)newNode;
}
void* ListItrRemove(ListItr _itr)
{
	void* tmp;
	if (_itr == NULL) { return NULL; }
	tmp = ((Node*)_itr)->m_data;
	((Node*)_itr)->m_prev->m_next = ((Node*)_itr)->m_next;
	((Node*)_itr)->m_next->m_prev = ((Node*)_itr)->m_prev;
	free(_itr);
	return tmp;
}


