#include "DoubleLL_tests.h"
int main()
{
	/*create*/
	testListCreate();
	/*destroy*/
	testListDestroy();
	/*push head*/
	testListPushHead();
	testListPushHeadNULL();
	/*push tail*/ 
	testListPushTail();
	testListPushTailNULL();
	/* pop head */
	testListPopHead();
	testListPopHeadEmpty();
	/* pop tail */
	testListPopTail();
	testListPopTailEmpty();
	/* is empty*/
	testListIsEmpty();
	/*count items*/
	testListCountItems();
	/* itrs */
	testListItrBegin();
	testListItrBeginEmpty();
	testListItrEndEmpty();
	testListItrEnd();

	testListItrNext();
	testListItrPrev();
	testListItrGet();
	testListItrGetEnd();
	testListItrEquals();

	testListItrSet();
	testListItrInsertBefore();
	
	testListItrRemove();
	
	/* count if forEach */
	testListItr_FindFirst();
	testListItr_CountIf();
	testListItr_ForEach();
	return 0;
}

void testListCreate();
