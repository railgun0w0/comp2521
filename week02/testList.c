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
	printf("Ready to test the first function DLListBefore\n");
	printf("Step 1: add line :death strand coming\n");
	DLListBefore(list,"death strand coming");
	printf("After the operation\n");
	showstate(list);
	assert (validDLList (list));



	freeDLList (list);
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
