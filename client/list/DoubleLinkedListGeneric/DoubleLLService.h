#ifndef __DOUBLELLSERVICE_H__
#define __DOUBLELLSERVICE_H__
/*typedef void* ListItr;*/
typedef struct Node Node;
typedef struct List List;

struct Node
{
	void*    m_data;
	Node*     m_next;
	Node*     m_prev;
	int m_existance;
};
struct List
{
	Node m_head; 
	Node m_tail;
	int m_existance;
};
#endif /* __DOUBLELLSERVICE_H__ */