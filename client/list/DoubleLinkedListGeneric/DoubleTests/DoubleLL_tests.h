#ifndef __DOUBLELL_TESTS_H__
#define __DOUBLELL_TESTS_H__
#include "../DoubleLLService.h"

/* getters */
Node *getListHead(List *_l);
Node *getListTail(List *_l);
Node *getNodeNext(Node *_n);
Node *getNodePrev(Node *_n);
void *getData(Node *_n);
int getNodeExistance(Node *_n);
int getListExistance(List *_l);
/* end getters */

/* list create */
void testListCreate();

/* destroy */
void testListDestroy();
/* push head */
void testListPushHead();
void testListPushHeadNULL();
/* push tail */
void testListPushTail();
void testListPushTailNULL();
/* pop head */
void testListPopHead();
void testListPopHeadEmpty();

/* pop tail */
void testListPopTail();
void testListPopTailEmpty();
/* count items */
void testListCountItems();
/* is empty */
void testListIsEmpty();

/* itrs */
void testListItrBegin();
void testListItrBeginEmpty();
void testListItrEndEmpty();
void testListItrEnd();

void testListItrNext();
void testListItrPrev();
void testListItrGet();
void testListItrGetEnd();
void testListItrEquals();

void testListItrSet();
void testListItrInsertBefore();

void testListItrRemove();

/* count if forEach */
void testListItr_FindFirst();
void testListItr_CountIf();
void testListItr_ForEach();

#endif /* __DOUBLELL_TESTS_H__ */