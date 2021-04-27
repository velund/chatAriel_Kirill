#include <stdio.h> /* printf */
#include <stdlib.h> /* size_t */
#include "DoubleLL_tests.h"
#include "../DoubleLL/DoubleLL.h"
#include "../DoubleLLItr/DoubleLLItr.h"
#include "../DoubleLLItr/DoubleLLItr2.h"
/* predicate , action */

int predicate(void *_el, void *_context)
{
	if ( *(int*)_el == 2 ) { return 1; }
	else return 0;
}

int printIntElement(void *_el, void *_context)
{
	if ( _el == NULL )
	{
		return 0;
	}
	printf("%d, ", *(int*)_el);
	return 1;
}

/*  */



void testListCreate()
{
	List *l;
	l = ListCreate();
	if (getListTail(l) == NULL || getListHead(l) == NULL) 
	{printf("testListCreate\t\t fail, on tail or head null\n"); ListDestroy(&l, NULL); return;}
	
	if (getListExistance(l) != EXISTANCE ) 
	{printf("testListCreate\t\t fail, on list ex. update\n"); ListDestroy(&l, NULL); return;}

	if (getNodeExistance (getListTail(l)) != EXISTANCE || getNodeExistance (getListHead(l)) != EXISTANCE) 
	{printf("testListCreate\t\t fail, on tail or head null\n"); ListDestroy(&l, NULL); return;}
	printf("testListCreate\t\t pass\n"); 
	ListDestroy(&l, NULL);
}

/* destroy */
void testListDestroy()
{
	List *l;
	l = ListCreate();
	ListDestroy(&l, NULL);
	if (l != NULL) { printf("testListDestroy\t\t fail, on list != NULL\n"); return; }
	printf("testListDestroy\t\t pass\n");
}

/* push head */
void testListPushHead()
{
	List *l;
	int a, b, *res;
	a=1;
	l = ListCreate();
	ListPushHead(l, &a);
	res = getData( getNodeNext(getListHead(l) ) );
	if (*res != 1)
	{printf("testListPushHead\t\t fail, on empty push\n" ); ListDestroy(&l, NULL); return;}
	b=2;
	ListPushHead(l, &b);
	res = getData( getNodeNext(getListHead(l) ) );
	if (*res != 2)
	{printf("testListPushHead\t\t fail, on push 2\n"); ListDestroy(&l, NULL);  return;}
	printf("after push head twice\t"); ListPrint(l);
	printf("testListPushHead\t pass\n"); 
	ListDestroy(&l, NULL);
}

void testListPushHeadNULL()
{
	List *l;
	int a, b=3;
	l = ListCreate();
	a=1;
	ListPushHead(l, &a);
	if ( ListPushHead(NULL, &b) != NULL_POINTER ) 
	{printf("testListPushHeadNULL\t, fail on push null\n"); ListDestroy(&l, NULL); return;} 
	printf("testListPushHeadNULL\t pass\n");
	ListDestroy(&l, NULL); 
}

/* push tail */
void testListPushTail()
{
	List *l;
	int a, b, *res;
	l = ListCreate();
	a=1; 
	ListPushTail(l, &a);
	res = getData( getNodePrev( getListTail(l) ) );
	if ( *res != 1 ) {printf("testListPushTail\t fail, on to empty value v: %d",*res ); ListDestroy(&l, NULL); return;}
	b=2;
	ListPushTail(l, &b);
	res = getData( getNodePrev( getListTail(l) ) );
	if ( *res != 2 ) {printf("testListPushTail\t fail, on value v: %d",*res ); ListDestroy(&l, NULL); return;}
	printf("after push tail twice\t"); ListPrint(l);
	printf("testListPushTail\t pass\n"); 
	ListDestroy(&l, NULL);
}

void testListPushTailNULL()
{
	List *l;
	int a=1, b=3;
	l = ListCreate();
	ListPushTail(l, &a);
	if (ListPushTail(NULL, &b) != NULL_POINTER ) {printf("testListPushTailNULL\t fail\n"); ListDestroy(&l, NULL); return;}
	ListDestroy(&l, NULL);
}


/* pop head */
void testListPopHead()
{
	List *l;
	int i, arr[3];
	int *value;
	l = ListCreate();
	for (i=0; i<3; i++)
	{
		arr[i] = i+1;
		ListPushTail(l, arr+i);
	}
	ListPrint(l);
	ListPopHead(l, (void**)&value);
	ListPrint(l);
	if (*value != 1) {printf("testListPopHead\t fail, on value: %d\n", *value); ListDestroy(&l, NULL); return;}
	printf("testListPopHead\t\t pass\n");
	ListDestroy(&l, NULL);
}
void testListPopHeadEmpty()
{
	List *l;
	int *value;
	l = ListCreate();
	if(ListPopHead(l, (void**)&value) != EMPTY_LIST ) {printf("testListPopHeadEmpty\t fail"); ListDestroy(&l, NULL); return;}
	printf("testListPopHeadEmpty\t pass\n"); 
	ListDestroy(&l, NULL);
}
/*pop tail*/
void testListPopTail()
{
	List *l;
	int *value, i, arr[3];
	l = ListCreate();
	for (i=0; i<3; i++)
	{
		arr[i] = i+1;
		ListPushTail(l, arr+i);
	}
	ListPrint(l);
	ListPopTail(l, (void**)&value);
	ListPrint(l);
	if (*value != 3) {printf("testListPopTail\t fail, on value: %d\n", *value); ListDestroy(&l, NULL); return;}
	printf("testListPopTail\t\t pass\n");
	ListDestroy(&l, NULL);
}
void testListPopTailEmpty()
{
	List *l;
	int *value;
	l = ListCreate();
	if(ListPopTail(l, (void**)&value) != EMPTY_LIST ) {printf("testListPopTailEmpty\t fail\n"); ListDestroy(&l, NULL); return;}
	printf("testListPopTailEmpty\t pass\n"); 
	ListDestroy(&l, NULL);
}
void testListCountItems()
{
	List *l;
	int a= 1,b=2, c=45, d= 78;
	l = ListCreate();
	if (ListSize(l) != 0 ) {printf("testListCountItems\t fail, on empty\n"); ListDestroy(&l, NULL); return; }
	ListPushTail(l,&a);
	ListPushTail(l,&b);
	ListPushHead(l, &c);
	ListPushHead(l, &d);
	if (ListSize(l) != 4 ) {printf("testListCountItems\t fail\n"); ListDestroy(&l, NULL); return; }
	printf("testListCountItems\t pass\n"); ListDestroy(&l, NULL);
}
void testListIsEmpty()
{
	List *l;
	int a=45;
	l = ListCreate();
	if (!ListIsEmpty(l)){printf("testListIsEmpty\t fail, on empty\n"); ListDestroy(&l, NULL); return; }
	ListPushHead(l, &a);
	if (ListIsEmpty(l)){printf("testListIsEmpty\t fail, on full\n"); ListDestroy(&l, NULL); return; }
	printf("testListIsEmpty\t\t pass\n"); ListDestroy(&l, NULL);
}

/*ListItr ListItrBegin(const List* _list)*/
void testListItrBegin()
{
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	if ( *(int*)(((Node*)itr)->m_data) != 6 ) 
	{ 
		printf("testListItrBegin\t, fail, onbegin value, value: %d\n", *(int*)(((Node*)itr)->m_data)) ;
	}else
	{
		printf("testListItrBegin\t pass\n") ;	
	}
	ListDestroy(&l, NULL);
}

void testListItrBeginEmpty()
{
	List *l;
	ListItr itr;
	l = ListCreate();
	itr = ListItrBegin(l);
	if (((Node*)itr)->m_next != ((Node*)itr) ) { printf("testListItrBeginEmpty\t fail\n"); }
	else{ printf("testListItrBeginEmpty\t pass\n"); }
	ListDestroy(&l, NULL);

}

void testListItrEndEmpty()
{
	List *l;
	ListItr itr;
	l = ListCreate();
	itr = ListItrEnd(l);
	if (((Node*)itr)->m_next != ((Node*)itr) ) { printf("testListItrBeginEmpty\t fail\n"); }
	else{ printf("testListItrBeginEmpty\t pass\n"); }
	ListDestroy(&l, NULL);
}
void testListItrEnd()
{
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrEnd(l);
	if ( *(int*)(((Node*)itr)->m_prev->m_data) != 2 || ((Node*)itr)->m_next !=  (Node*)itr ) 
	{ 
		printf("testListItrEnd\t\t, fail, onbegin value, value: %d\n", *(int*)(((Node*)itr)->m_data)) ;
	}else
	{
		printf("testListItrEnd\t\t pass\n") ;	
	}
	ListDestroy(&l, NULL);
}

void testListItrNext()
{
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	itr = ListItrNext(itr);
	if ( *(int*)(((Node*)itr)->m_data) != 1 ) 
	{ 
		printf("testListItrNext\t\t, fail, onbegin value, value: %d\n", *(int*)(((Node*)itr)->m_data)) ;
	}else
	{
		printf("testListItrNext\t\t pass\n") ;	
	}
	ListDestroy(&l, NULL);
}
void testListItrPrev()
{
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	itr = ListItrNext(itr);
	itr = ListItrNext(itr);
	itr = ListItrPrev(itr);
	if ( *(int*)(((Node*)itr)->m_data) != 1 ) 
	{ 
		printf("testListItrPrev\t\t, fail, onbegin value, value: %d\n", *(int*)(((Node*)itr)->m_data)) ;
	}else
	{
		printf("testListItrPrev\t\t pass\n") ;	
	}
	ListDestroy(&l, NULL);
}

void testListItrGet()
{
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	if ( *(int*)ListItrGet(itr) != 6 ) 
	{ 
		printf("testListItrGet\t\t, fail, onbegin value, value: %d\n", *(int*)(((Node*)itr)->m_data)) ;
	}else
	{
		printf("testListItrGet\t\t pass\n") ;	
	}
	ListDestroy(&l, NULL);
}
void testListItrGetEnd()
{
	void* a;
	List *l;
	ListItr itr;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrEnd(l);
	a = ListItrGet(itr);
	if (a != NULL) { printf("testListItrGetEnd\t, fail\n"); }
	else
	{ printf("testListItrGetEnd\t pass\n"); }
	ListDestroy(&l, NULL);
}

void testListItrEquals()
{
	List *l;
	ListItr itr1, itr2;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr1 = ListItrEnd(l);
	itr1 = ListItrPrev(itr1);
	itr2 = ListItrBegin(l);
	itr2 = ListItrNext(itr2);
	itr2 = ListItrNext(itr2);
	itr2 = ListItrNext(itr2);
	
	if (itr1 != itr2) { printf("testListItrEquals\t, fail\n"); }
	else
	{ printf("testListItrEquals\t pass\n"); }
	ListDestroy(&l, NULL);
}

void testListItrSet()
{
	List *l;
	ListItr itr;
	int newVal = 24;
	void *returned;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	itr = ListItrNext(itr);
	returned = ListItrSet(itr, (void*)&newVal);
	if ( *(int*)returned != 1 )
	{
		printf("testListItrSet\t\t fail, on return value value: %d\n", *(int*)returned);
	}else 
	if (*(int*)ListItrGet(itr) != 24)
	{
		printf("testListItrSet\t\t fail, on return set value value: %d\n", *(int*)ListItrGet(itr));
	}else
	{ printf("testListItrSet\t\t pass\n"); }
	ListDestroy(&l, NULL);
}

void testListItrInsertBefore()
{
	List *l;
	ListItr itr, itr2;
	int newVal = 24;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	itr2 = ListItrInsertBefore(itr, (void*)&newVal);
	if ( *(int*)ListItrGet(itr2) != 24 )
	{
		printf("testListItrInsertBefore\t\t fail, on returned nodes value value: %d\n", *(int*)ListItrGet(itr));
	}else 
	{
		itr = ListItrBegin(l);
		if (*(int*)ListItrGet(itr) != 24)
		{
			printf("testListItrInsertBefore\t\t fail, on begin value: %d\n", *(int*)ListItrGet(itr));
		}
	}
	
	for (i=0; i<5; i++)
	{
		printf("%d,", *(int*)ListItrGet(itr));
		itr = ListItrNext(itr);
	}
	putchar('\n');

	printf("testListItrInsertBefore\t\t pass\n"); 
	ListDestroy(&l, NULL);	
}

void testListItrRemove()
{
	List *l;
	ListItr itr;
	int removed;
	size_t i;
	int arr[4] = { 2,5,1,6 };
	l = ListCreate();
	for (i=0; i<4; i++)
	{
		ListPushHead(l, (void* )(arr+i));
	}
	itr = ListItrBegin(l);
	removed = *(int*)ListItrRemove(itr);
	itr = ListItrBegin(l);
	printf("after remove:\n");
	for (i=0; i<3; i++)
	{
		printf("%d,", *(int*)ListItrGet(itr));
		itr = ListItrNext(itr);
	}
	putchar('\n');
	if (removed != 6){ printf("testListItrRemove\t fail, on removed value, value: %d\n", removed); }
	else 
	{ printf("testListItrRemove\t pass\n");}
	ListDestroy(&l, NULL);
}

/* findFirst, counIf, forEach */
void testListItr_FindFirst()
{
	List *l;
	ListItr start, end, found;
	size_t i;
	int arr[8] = { 2,5,1,6,2,8,2,15};
	l = ListCreate();
	for (i=0; i<8; i++)
	{
		ListPushTail(l, (void* )(arr+i));
	}
	start = ListItrBegin(l);
	start = ListItrNext(start);
	end = ListItrEnd(l);
	found = ListItr_FindFirst(start, end, predicate, NULL);
	if (*(int*)ListItrGet( ListItrPrev(found) ) !=6 || *(int*)ListItrGet( ListItrNext(found) ) !=8 )
	{
		printf("testListItr_FindFirst\t fail, on prev next vals\n");
		ListDestroy(&l, NULL);
		return;
	}
	printf("testListItr_FindFirst\t pass\n");
	ListDestroy(&l, NULL);
}

void testListItr_CountIf()
{
	List *l;
	ListItr start, end;
	size_t i, counter;
	int arr[8] = { 2,5,1,6,2,8,2,15};
	l = ListCreate();
	for (i=0; i<8; i++)
	{
		ListPushTail(l, (void* )(arr+i));
	}
	start = ListItrBegin(l);
	end = ListItrEnd(l);
	counter = ListItr_CountIf(start, end, predicate, NULL);
	if (counter != 3 )
	{
		printf("testListItr_CountIf\t fail, on counter\n");
		ListDestroy(&l, NULL);
		return;
	}
	printf("testListItr_CountIf\t pass\n");
	ListDestroy(&l, NULL);
}

void testListItr_ForEach()
{
	List *l;
	ListItr start, end;
	size_t i;
	int arr[8] = { 2,5,1,6,2,8,2,15};
	l = ListCreate();
	for (i=0; i<8; i++)
	{
		ListPushTail(l, (void* )(arr+i));
	}
	start = ListItrBegin(l);
	start = ListItrNext(start);
	end = ListItrEnd(l);
	/*end = ListItrPrev(end);*/
	end = ListItr_ForEach(start, end, printIntElement, NULL);
	putchar('\n');
	printf("in end: %d\n", *(int*)ListItrGet(end));
	printf("testListItr_CountIf\t pass\n");
	ListDestroy(&l, NULL);
}


