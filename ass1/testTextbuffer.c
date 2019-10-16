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
	TB tb1 = newTB("asdasdasd\n123123123\nilove\nmaybe\n");
	assert(linesTB(tb1) == 4);
	char *text1 = dumpTB(tb1, true); // Don't show line numbers
	
	assert(strcmp("1. asdasdasd\n2. 123123123\n3. ilove\n4. maybe\n", text1) == 0);
	
	addPrefixTB(tb1, 1, 4, "1");
	deleteTB(tb1,1,2);
	char *text2 = dumpTB(tb1, false); // Don't show line numbers
	assert(strcmp("1ilove\n1maybe\n",text2) == 0);
	assert(linesTB(tb1) == 2);
	TB tb2 = newTB("tb2\ntb2\ntb2\ntb2\n");
	printf("%d111\n",linesTB(tb2));
	mergeTB(tb1, 1, tb2);
	printf("%d111\n",linesTB(tb2));
	assert(linesTB(tb2) == 6);
	free(text1);
	free(text2);

	releaseTB(tb1);
	
	
	printf("newTB tests passed!\n");
}

// TODO: Add more test functions
