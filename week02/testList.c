// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

typedef struct DLListNode {
	char *value;        /**< value of this list item (string) */
	struct DLListNode *prev;
	                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode;

typedef struct DLListRep {
	size_t nitems;      /**< count of items in list */
	DLListNode *first;  /**< first node in list */
	DLListNode *curr;   /**< current node in list */
	DLListNode *last;   /**< last node in list */
} DLListRep;


void showcurrentvalue(DLList L);
void showstate(DLList L);

int main (void)
{
	// DLList myList = getDLList (stdin);
	// putDLList (stdout, myList);
	// assert (validDLList (myList));
	// TODO: more tests needed here
	printf("Test set up\n");
	printf("create empty list\n");
	DLList list = newDLList();
	assert (validDLList (list));
	printf("validDLList pass\n");
	printf("show the inital state of empty list\n");
	showstate(list);

	printf("Set up complete,start test the first function\n");
	printf("-----------Start to test DLListBefore--------------------\n");
	printf("Step 1: add line to the empty list :death stranding coming\n");
	DLListBefore(list,"death stranding coming");
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("Step 2: add another line with one line:buybuybuybuy\n");
	showstate(list);
	DLListBefore(list,"buybuybuybuy");
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("Step 3: add another line before the fist line of a list:buy in 2019.11.8\n");
	showstate(list);
	DLListBefore(list,"buy in 2019.11.8");
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("Step 4: add another line before the middle line of a list:perorder now\n");
	showstate(list);
	printf("change current position to the second line\n");
	DLListMove(list,1);
	printf("After the operation\n");
	showstate(list);
	printf("add line\n");
	DLListBefore(list,"perorder now");
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("-----------passed all test about DLListBefore-----------\n");

	printf("-----------Start to test DLListAfter--------------------\n");
	printf("Test set up\n");
	printf("create empty list\n");
	DLList newlist = newDLList();
	assert (validDLList (newlist));
	printf("validDLList pass\n");
	printf("show the inital state of empty list\n");
	showstate(list);
	printf("Set up complete,start!\n");
	printf("---------------------------------------------------------\n");

	printf("Step1: add one line to the empty list");
	printf("add:i love video games!\n");
	DLListAfter(newlist,"i love video games!");
	printf("After the operation\n");
	showstate(newlist);
	assert (validDLList (newlist));
	printf("validDLList pass\n");

	printf("Step2: add new line to the one line");
	showstate(newlist);
	printf("add:i hate video games!\n");
	DLListAfter(newlist,"i hate video games!");
	printf("After the operation\n");
	showstate(newlist);
	assert (validDLList (newlist));
	printf("validDLList pass\n");

	printf("Step3: add new line to the end of list");
	showstate(newlist);
	printf("add:i am not good at video games!\n");
	DLListAfter(newlist,"i am not good at video games!");
	printf("After the operation\n");
	showstate(newlist);
	assert (validDLList (newlist));
	printf("validDLList pass\n");
	
	printf("Step 4: add another line after the middle line of a list\n");
	showstate(newlist);
	printf("change current position to the second line\n");
	DLListMove(newlist,-1);
	printf("After the operation\n");
	showstate(newlist);
	printf("add line:i am good at video games!\n");
	DLListAfter(newlist,"i am good at video games!");
	printf("After the operation\n");
	showstate(newlist);
	assert (validDLList (newlist));
	printf("validDLList pass\n");

	printf("-----------passed all test about DLListAfter-----------\n");

	printf("-----------Start to test DllistDelete------------------\n");
	printf("Show the inite state of list");
	showstate(list);
	printf("Step1: Delete the head of list\n");
	printf("change the current became the head\n");
	DLListMoveTo(list,1);
	printf("After the operation\n");
	showstate(list);
	printf("delete the head\n");
	DLListDelete(list);
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("Step2: Delete the middle of list\n");
	showstate(list);
	printf("change the current became the middle\n");
	DLListMoveTo(list,2);
	printf("After the operation\n");
	showstate(list);
	printf("delete the middle\n");
	DLListDelete(list);
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");
	
	printf("Step3: Delete the end of list\n");
	showstate(list);
	printf("delete the end\n");
	DLListDelete(list);
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");
	
	printf("Step4: Delete the last line of list\n");
	showstate(list);
	printf("delete the last line\n");
	DLListDelete(list);
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");

	printf("Step5: Delete if is an empty list\n");
	showstate(list);
	printf("try to use delete\n");
	DLListDelete(list);
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));
	printf("validDLList pass\n");
	
	printf("PASSED ALL TEST GOOD JOB!!\n");

	freeDLList(list);
	freeDLList(newlist);
	return EXIT_SUCCESS;
}

void showcurrentvalue(DLList L){
	if(L->nitems == 0){
		printf("The current value does not exist,the list is empty\n");
	}else{
		printf("%s\n",L->curr->value);
	}
}

void showstate(DLList L){
	printf("show the content of list:\n");
	if(L->nitems == 0){
		printf("There is empty list\n");
	}else{
		putDLList(stdout,L);
	}
	printf("show curr:");
	showcurrentvalue(L);
	printf("show nitems:");
	printf("%zu\n",L->nitems);
}
