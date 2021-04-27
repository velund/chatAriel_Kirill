#include <stdio.h>
#include "DoubleLL.h"
#include "../DoubleLLService.h"
/* assist funcs */
static void edNull(void *_item);
/* end assist funcs */
List* ListCreate(void)
{
	List *l = (List*)malloc(sizeof(List)); 
	if (l == NULL ) {return NULL;}
	l->m_head.m_next = &l->m_tail; 
	l->m_head.m_prev = &l->m_head;
	l->m_tail.m_prev = &(l->m_head); 
	l->m_tail.m_next = &l->m_tail;
	l->m_existance = EXISTANCE;
	l->m_head.m_existance = EXISTANCE;
	l->m_tail.m_existance = EXISTANCE;
	l->m_head.m_data = NULL;
	l->m_tail.m_data = NULL;
	return l;
}
void ListDestroy(List **_list, _elementDestroy _ed)
{
	Node *tmp, *n;
	_elementDestroy ed;
	if (_list == NULL) {return;}
	if (*_list == NULL) { return; }
	if ( &((*_list)->m_head) == NULL || &((*_list)->m_tail) == NULL ) {return;}
	if (_ed == NULL) {ed = edNull;}
	else { ed = _ed; }
	tmp = (*_list)->m_head.m_next;
	while (tmp != &(*_list)->m_tail)
		{
			n = tmp->m_next;
			tmp->m_existance = 0;
			if (tmp != NULL) {(*ed)(tmp->m_data); free(tmp);}
			tmp = n;
		}
	
	(*_list)->m_head.m_existance = 0;
	(*_list)->m_tail.m_existance = 0;
	(*_list)->m_existance = 0;
	free(*_list);
	*_list = NULL;
}



ADTErr ListPushHead(List* _list, void *_data)
{
	Node *p;
	Node *t;
	if (_list == NULL || _data == NULL) {return NULL_POINTER;}
	if ( (p = malloc(sizeof(Node))) == NULL) {return MALLOC_FAIL;}
	p->m_existance = EXISTANCE;
	p->m_data = _data;
	t = _list->m_head.m_next;
	t->m_prev = &_list->m_head;  
	p->m_prev = &_list->m_head; 
	p->m_next = t;
	t->m_prev->m_next = p;
	t->m_prev = p;
	return SUCCESS;
}
ADTErr ListPushTail(List* _list, void *_data)
{
	Node *p;
	Node *t;
	if (_list == NULL) {return NULL_POINTER;}
	if ( (p = malloc(sizeof(Node))) == NULL) {return MALLOC_FAIL;}
	t = &_list->m_tail;
	p->m_prev = t->m_prev;
	p->m_next = t;
	t->m_prev->m_next = p;
	t->m_prev = p;
	p->m_data = _data;
	return SUCCESS;
}
ADTErr ListPopHead(List* _list, void **_data)
{
	Node *n;
	if (_list == NULL) {return NULL_POINTER;}
	if ( &_list->m_head == NULL || &_list->m_tail == NULL ) {return NO_SUCH_LIST;}
	if (ListIsEmpty(_list)) {return EMPTY_LIST;}
	n = _list->m_head.m_next;
	*_data = n->m_data;
	_list->m_head.m_next = n->m_next;
	n->m_next->m_prev = &_list->m_head;
	if (n != NULL) {free(n);}
	return SUCCESS;

}
ADTErr ListPopTail(List* _list, void **_data)
{
	Node *n;
	if (_list == NULL) {return NULL_POINTER;}
	if ( &_list->m_head == NULL || &_list->m_tail == NULL ) {return NO_SUCH_LIST;}
	if (ListIsEmpty(_list)) {return EMPTY_LIST;}
	n = _list->m_tail.m_prev;
	*_data = n->m_data;
	_list->m_tail.m_prev = n->m_prev;
	n->m_prev->m_next = &_list->m_tail;
	if (n != NULL) {free(n);}
	return SUCCESS;
}
size_t ListSize(const List* _list)
{
	size_t counter=0;
	Node *n;
	if (_list == NULL) { return counter; }
	if ( &_list->m_head == NULL || &_list->m_tail == NULL ) {return counter;}
	n = _list->m_head.m_next;
	while ( n != &_list->m_tail )
	{
		counter++;
		n = n->m_next;
	}
	return counter;
}
int ListIsEmpty(List* _list)
{
	if (_list == NULL) { return EMPTY; }
	if ( &_list->m_head == NULL || &_list->m_tail == NULL ) {return EMPTY;}
	if ( _list->m_head.m_next == &_list->m_tail ) {return EMPTY;}
	return NOT_EMPTY;
}

/* ListPrint is only for debug */
void ListPrint(List* _list)
{
	Node *n;
	if (_list == NULL) { return; }
	if ( &_list->m_head == NULL || &_list->m_tail == NULL ) {return;}
	n = _list->m_head.m_next;
	while ( n != &_list->m_tail )
	{
		printf("%d,", *((int*)n->m_data));
		n = n->m_next;
	}
	putchar('\n');
}

/* assist funcs */
static void edNull(void *_item){}
/* end assist funcs */

/* getters */
Node *getListHead(List *_l)
{
	return &_l->m_head;
}
Node *getListTail(List *_l)
{
	return &_l->m_tail;
}

Node *getNodeNext(Node *_n)
{
	return _n->m_next;
}
Node *getNodePrev(Node *_n)
{
	return _n->m_prev;
}

void *getData(Node *_n)
{
	return _n->m_data;
}
int getNodeExistance(Node *_n)
{
	return _n->m_existance;
}
int getListExistance(List *_l)
{
	return _l->m_existance;
}



