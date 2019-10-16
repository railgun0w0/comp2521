#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textbuffer.h"

struct textbuffer {
	int row;
	struct textnode *first;
	struct textnode *last;
};

typedef struct textnode {
	char *string;
	int line;
	struct textnode *next;
	struct textnode *prev;
} textnode; 

TB emptyTB (void) {
	TB new = malloc(sizeof *new);
	if (new == NULL) printf("couldn't allocate textbuffer");
	new->row = 0;
	new->first = new->last = NULL;
	return new;
}

textnode *newnode (char *string) {
	textnode *newTN = malloc(sizeof *newTN);
	if (newTN == NULL) printf("couldn't allocate textnode");
	newTN->next = newTN->prev = NULL;
	newTN->string = string;
	newTN->line = 0;
	return newTN;
}

void insertnode(TB T, textnode *node) {
	if(T->first == NULL){
		T->first = T->last = node;
		node->line = 1;
	} else {
		textnode *curr = T->last;
		curr->next = node;
		node->prev = curr;
		T->last = node;
		node->line = curr->line + 1;
	}
	T->row++;
}
/**
 * Allocate a new textbuffer whose contents is initialised with the text
 * in the given string.
 */

void copycopycopy(char *sourse, char *really, int prev, int goal){
	int i = 0;
	while(prev < goal){
		really[i] = sourse[prev];
		i++;
		prev++;
	}
	really[i] = '\n';
	really[i+1] = '\0';
}

void tokenise(char *text, TB T){
	int flag = 0;
	int move = 0;
    while(text[move] != '\0'){
        if(text[move] == '\n'){
		    char *string = malloc((move - flag)*sizeof(char)+1+1);
            copycopycopy(text,string,flag,move);
		    textnode *node = newnode(string);
		    insertnode(T, node);
			flag = move + 1;
		}    
		move++;
	}
}


/*
start with a pointer to the begining of the string
iterate character by character, looking for your delimiter
each time you find one, you have a string from the last position of the length in difference - do what you want with that
set the new start position to the delimiter + 1, and the go to step 2.
*/

TB newTB(char *text) {
	// Create a empty TB
	TB new = emptyTB();
	if(text == NULL) return new;
	tokenise(text,new);
	return new;
}

void freetextnode(textnode *node) {
	if (node == NULL) return;
	free (node->string);
	free (node);
}
/**
 * Free  the  memory occupied by the given textbuffer. It is an error to
 * access the buffer afterwards.
 */
void releaseTB (TB tb) {
	if (tb == NULL){
		printf("access the buffer after FREE");
		exit(1);
	}
	textnode *curr = tb->first;
	while (curr != NULL) {
		textnode *next = curr->next;
		freetextnode (curr);
		curr = next;
	}
	free (tb);
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */

int countsize(TB tb){
	int size = 0;
	textnode *curr = tb->first;
	while(curr != NULL){
		size += strlen(curr->string);
		curr = curr->next;
	}
	return size;
}

char *dumpTB(TB tb, bool showLineNumbers) {
	if(tb == NULL){
		printf("textbuffer is null\n");
		return NULL;
	}
	textnode *curr = tb->first;
	char *str = NULL;
	int count = 1;
	int size;
	char *number = NULL;
	if(curr == NULL) {
	    str = malloc(sizeof(char)); 
	    str[0] = '\0';   
	    return str;
	}
	if(showLineNumbers == true){
		//add prefix
		size = countsize(tb) + 3*tb->row;
		str = (char *)malloc(sizeof(char)*size + 1);
		str[0] = '\0';
		while(curr != NULL){
			number = malloc(sizeof(char)*4);
			sprintf(number, "%d. ", count);
			count++;
			strcat(str,number);
			free(number);
			strcat(str,curr->string);
			curr = curr->next;
		}
		number = NULL;
		return str;
	}else{
		size = countsize(tb);
		str = (char *)malloc(sizeof(char)*size+1);
		str[0] = '\0';
		while(curr != NULL){
			strcat(str,curr->string);
			curr = curr->next;
		}
	}
	return str;
}

/**
 * Return the number of lines of the given textbuffer.
 */
int linesTB(TB tb) {
	return tb->row;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB(TB tb, int from, int to, char *prefix) {
    if(to < from || from < 0 || to > tb->row){
		fprintf(stderr,"out of range\n");
		abort();
	}
	int size = strlen(prefix);
	textnode *curr = tb->first;
	while(curr->line != from){
		curr = curr->next;
	}
	int start = curr->line;
	while(start <= to){
		//cpoy start
		char *newstring = malloc(sizeof(char)*size + 1 + sizeof(char)*strlen(curr->string));
		newstring[0] = '\0';
		strcat(newstring, prefix);
		strcat(newstring, curr->string);
		free(curr->string);
		curr->string = newstring;
		curr = curr->next;
		start++;
	}
}

/**
 * Merge 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the merged-in lines from 'tb2'.
 *   - 'tb2' can't be used anymore (as if we had used releaseTB() on it)
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void mergeTB(TB tb1, int pos, TB tb2) {
	

}

/**
 * Copy 'tb2' into 'tb1' at line 'pos'.
 * - After this operation:
 *   - What was at line 1 of 'tb2' will now be at line 'pos' of 'tb1'.
 *   - Line  'pos' of 'tb1' will moved to line ('pos' + linesTB('tb2')),
 *     after the pasted-in lines from 'tb2'.
 *   - 'tb2' is unmodified and remains usable independent of tb1.
 * - The program should abort() with an error message if 'pos' is out of
 *   range.
 */
void pasteTB(TB tb1, int pos, TB tb2) {

}

/**
 * Cut  the lines between and including 'from' and 'to' out of the given
 * textbuffer 'tb' into a new textbuffer.
 * - The result is a new textbuffer (much as one created with newTB()).
 * - The cut lines will be deleted from 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
TB cutTB(TB tb, int from, int to) {
	return NULL;
}

/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	return NULL;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {
	if(to < from || from < 0 || to > tb->row){
		fprintf(stderr,"out of range\n");
		abort();
	}
	textnode *curr = tb->first;
	while(curr->line != from){
		curr = curr->next;
	}
	textnode *newhead = curr->prev;
	int start = curr->line;
	while(start <= to){
		textnode *next = curr->next;
		free(curr->string);
		free(curr);
		start++;
		curr = next;
	}
	if(from == 1 && to == tb->row){
		tb->row = 0;
		tb->first = tb->last = NULL;
	}else if(to == tb->row){
		tb->last = newhead;
		newhead->next = NULL;
	}else if(from == 1){
		tb->first = curr;
		curr->line = 1;
		curr = curr->next;
		while(curr != NULL){
			curr->line = curr->prev->line + 1;
			curr = curr->next;
		}
		tb->row = tb->last->line;
	}else{
		newhead->next = curr;
		curr->prev = newhead;
		while(curr != NULL){
			curr->line = curr->prev->line + 1;
			curr = curr->next;
		}
		tb->row = tb->last->line;
	}
}

/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb) {

}

////////////////////////////////////////////////////////////////////////
// Bonus challenges

char *diffTB(TB tb1, TB tb2) {
	return NULL;
}

void undoTB(TB tb) {

}

void redoTB(TB tb) {

}
