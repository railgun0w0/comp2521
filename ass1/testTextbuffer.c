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
struct textbuffer {
	int row;
	struct textnode *first;
	struct textnode *last;
};

typedef struct textnode {
	char *string;
	struct textnode *next;
	struct textnode *prev;
} textnode; 


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
	TB tb1 = newTB("hello there,\nhow\nare\nthings\n");
	//assert(linesTB(tb1) == 4);

    printf("%s",tb1->first->string);
	//char *text1 = dumpTB(tb1, false); // Don't show line numbers
	//assert(strcmp("hello there,\nhow\nare\nthings\n", text1) == 0);
	//free(text1);

	releaseTB(tb1);
	
	
	printf("newTB tests passed!\n");
}

// TODO: Add more test functions
