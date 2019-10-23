// testTextbuffer.c
// A stub file for you to test your textbuffer implementation.
// Note that you will NOT be submitting this - this is simply for you to
// test  your  functions as you go. We will have our own testTexbuffer.c
// for testing your submission.
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "textbuffer.h"

static void testNewTB(void);

// TODO: Add more function prototypes


int main(void) {
	
	testNewTB();
	
	// TODO: Call more test functions
	
	printf("All tests passed! You are awesome!\n");
}

static void testNewTB(void) {
	printf("-----------------------------------------\n"
	       "               newTB tests               \n"
	       "-----------------------------------------\n");

	// Calling dumpTB immediately after newTB, without modifying the TB
	TB tb1 = newTB("#############_a*s*d*as_*asd*_*_*_____da__sd_a_asdasdasd\n");
	assert(linesTB(tb1) == 1);
	// Don't show line numbers
	formRichText(tb1);
	//assert(strcmp("1. asdasdasd\n2. 123123123\n3. ilove\n4. maybe\n", text1) == 0);
	
	//addPrefixTB(tb1, 1, 1, "i am gay ");
	
	char *text1 = dumpTB(tb1, false);
	
	printf("after rich :%s",text1);
	//deleteTB(tb1,1,2);
	//char *text2 = dumpTB(tb1, false); // Don't show line numbers
	//assert(strcmp("1ilove\n1maybe\n",text2) == 0);
	//assert(linesTB(tb1) == 2);
	//TB tb2 = newTB("1tb2\n2tb2\n3tb2\n4tb2\n");
	//mergeTB(tb1, 1, tb2);
	//assert(linesTB(tb1) == 6);
	free(text1);
	//free(text2);
	//formRichText(tb1);
	//char *text1 = dumpTB(tb1, true);
	//printf("%s",text1);
	//free(text1);
	releaseTB(tb1);
	printf("newTB tests passed!\n");
}

// TODO: Add more test functions

bool validDLList (TB L)
{
	if (L == NULL) {
		fprintf (stderr, "DLList is null\n");
		return false;
	}

	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL) {
			fprintf (stderr, "Non-null pointers in empty list\n");
			return false;
		}
	} else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL ) {
			fprintf (stderr, "Null pointers in non-empty list\n");
			return false;
		}
	}

	// check scanning forward through list
	int count = 0;
	for (textnode *curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf (
				stderr,
				"Invalid forward link into node (%s)\n",
				curr->string);
			return 0;
		}

		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf (
				stderr,
				"Invalid backward link into node (%s)\n",
				curr->string);
			return 0;
		}
		count++;
	}

	if (count != L->row) {
		fprintf (
			stderr,
			"Forward count mismatch; counted=%d, row=%d\n",
			count,
			L->row);
		return false;
	}

	// check scanning backward through list
	count = 0;
	for (textnode *curr = L->last; curr != NULL; curr = curr->prev)
		count++;

	if (count != L->row) {
		fprintf (
			stderr,
			"Backward count mismatch; counted=%d, rows=%d\n",
			count,
			L->row);
		return false;
	}
	// nothing went wrong => must be ok
	return true;
}