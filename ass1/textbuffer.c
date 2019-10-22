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

void updateline(TB tb);

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

void copycopycopy(char *source, char *really, int prev, int goal){
	int i = 0;
	while(prev < goal){
		really[i] = source[prev];
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
	if(text == NULL) {
		fprintf(stderr,"text is null\n");
		abort();
	}

	if(text[0] == '\0') return new;
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
		fprintf(stderr,"tb is null\n");
		abort();
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
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	int size = 0;
	textnode *curr = tb->first;
	while(curr != NULL){
		size += strlen(curr->string);
		curr = curr->next;
	}
	return size;
}

char *dumpTB(TB tb, bool showLineNumbers) {
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	int numbersize;
	textnode *curr = tb->first;
	char *str = NULL;
	int size;
	if(curr == NULL) {
	    str = malloc(sizeof(char)); 
	    str[0] = '\0';   
	    return str;
	}
	if(showLineNumbers == true){
		//add prefix
		int totalnumbersize = 0;
		int i = 1;
		while(i <= tb->row){
			char *tmp = malloc(10);
			tmp[0] = '\0';
			sprintf(tmp, "%d. ", i);
			numbersize = strlen(tmp);
			totalnumbersize += numbersize;  
			free(tmp);
			i++;
		}
		int strsiz = countsize(tb) + totalnumbersize;
		char *str = malloc(sizeof(char)*strsiz + 1);
		str[0] = '\0';
		while(curr != NULL){
			char *tmp2 = malloc(10);
			sprintf(tmp2,"%d. ",curr->line);
			strcat(str,tmp2);
			strcat(str,curr->string);
			free(tmp2);
			curr = curr->next;
		}
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
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	return tb->row;
}

/**
 * Add a given prefix to all lines between 'from' and 'to', inclusive.
 * - The  program  should abort() wih an error message if 'from' or 'to'
 *   is out of range. The first line of a textbuffer is at position 1.
 */
void addPrefixTB(TB tb, int from, int to, char *prefix) {
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
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
		//copy start
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
	if(tb1 == NULL|| tb2 == NULL) {
		fprintf(stderr,"textbuffer does not exist\n");
		abort();
	}
	if(pos < 0 || pos > tb1->row+1){
		fprintf(stderr,"out of range\n");
		abort();
	}
	if(tb1 == tb2) return;
	
	if(tb1->first == NULL && tb2->first == NULL){
		free(tb2);
		return;
	}else if(tb1->first == NULL && pos == 1){
		tb1->first = tb2->first;
		tb1->last = tb2->first;
		tb1->row = tb2->row;
		free(tb2);
		return;
	}else if(tb2->first == NULL){
		free(tb2);
		return;
	}
	//tb1 || tb2 empty tb1&tb2
	textnode *tb1curr = tb1->first;
	textnode *tb2curr = tb2->first;
	if(pos == (tb1->row + 1)){
		while(tb2curr != NULL){
			textnode *new = newnode(tb2curr->string);
			insertnode(tb1,new);
			tb2curr = tb2curr->next;
		}
		textnode *freenode = tb2->first;
		while(freenode != NULL){
			textnode *freenext = freenode->next;
			free(freenode);
			freenode = freenext;
		}
		free(tb2);
		return;
	}

	while(tb1curr->line != pos){
		tb1curr = tb1curr->next;
	}
	textnode *newhead = tb1curr->prev;
	textnode *tail = tb1curr;
	textnode *reallyend = tb1->last;
	while(tb2curr != NULL){
		textnode *new = newnode(tb2curr->string);
		insertnode(tb1,new);
		tb2curr = tb2curr->next;
	}
	textnode *tb2head = reallyend->next;
	textnode *tb2end = tb1->last;
	if(pos == (tb1->row + 1)){
		//just free
	}
	else if (pos == 1){
		tb1->first = tb2head;
		tb1->last = reallyend;
		tb2head->prev = NULL;
		reallyend->next = NULL;
		tb2end->next = tail;
		tail->prev = tb2end;
	}else{
		tb2head->prev = newhead;
		newhead->next = tb2head;
		tb2end->next = tail;
		tail->prev = tb2end;
	}
	updateline(tb1);
	textnode *freenode = tb2->first;
	while(freenode != NULL){
		textnode *freenext = freenode->next;
		free(freenode);
		freenode = freenext;
	}
	free(tb2);
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
	if(tb1 == NULL|| tb2 == NULL) {
		fprintf(stderr,"textbuffer does not exist\n");
		abort();
	}
    if(pos < 0 || pos > tb1->row+1){
		fprintf(stderr,"out of range\n");
		abort();
	}
	if(tb1->first == NULL && tb2->first == NULL){
		return;
	}else if(tb2->first == NULL){
		return;
	}else if(tb1->first == NULL && pos == 1){
		textnode *tb2curr = tb2->first;
		while(tb2curr != NULL){
			char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
			string[0] = '\0';
			strcpy(string,tb2curr->string);
			textnode *new = newnode(string);
			insertnode(tb1,new);
			tb2curr = tb2curr->next;	
		}
		updateline(tb1);
		return;
	}else{
		if(pos == 1){
			textnode *head = tb1->first;
			textnode *tail = tb1->last;
			textnode *tb2curr = tb2->first;
			while(tb2curr != NULL){
				char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
				string[0] = '\0';
				strcpy(string,tb2curr->string);
				textnode *new = newnode(string);
				insertnode(tb1,new);
				tb2curr = tb2curr->next;
			}
			textnode *copyend = tb1->last;
			tb1->first = tail->next;
			tb1->first->prev = NULL;
			tb1->last = tail;
			tb1->last->next = NULL;
			copyend->next = head;
			head->prev = copyend;
			updateline(tb1);
			return;
		}else if(pos == (tb1->row + 1)){
			textnode *tb2curr = tb2->first;
			while(tb2curr != NULL){
				char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
				string[0] = '\0';
				strcpy(string,tb2curr->string);
				textnode *new = newnode(string);
				insertnode(tb1,new);
				tb2curr = tb2curr->next;	
			}
			updateline(tb1);
			return;
		}else{
			textnode *tb1curr = tb1->first;
			while(tb1curr->line != pos){
				tb1curr = tb1curr->next;
			}
			textnode *tb1frontbreak = tb1curr->prev;
			textnode *tb1backbreak = tb1curr;
			textnode *tb2curr = tb2->first;
			textnode *tb1bfeend = tb1->last;
			while(tb2curr != NULL){
				char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
				string[0] = '\0';
				strcpy(string,tb2curr->string);
				textnode *new = newnode(string);
				insertnode(tb1,new);
				tb2curr = tb2curr->next;	
			}
			tb1frontbreak->next = tb1bfeend->next;
			tb1frontbreak->next->prev = tb1frontbreak;
			tb1->last->next = tb1backbreak;
			tb1backbreak->prev = tb1->last;
			updateline(tb1);
		}
	}

}
void updateline(TB tb){
	tb->first->line = 1;
	textnode *curr = tb->first->next;
	while(curr != NULL){
		curr->line = curr->prev->line + 1;
		curr = curr->next;
	}
	curr = tb->first;
	while(curr ->next != NULL){
		curr = curr->next;
	}
	tb->last = curr;
	tb->row = tb->last->line;
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
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	if(to < from || from < 0 || to > tb->row){
		fprintf(stderr,"out of range\n");
		abort();
	}
	TB new = emptyTB();
	if(from == 1 && to == tb->row){
		new->first = tb->first;
		new->last = tb->last;
		new->row = tb->row;
		tb->first = NULL;
		tb->last = NULL;
		tb->last = NULL;
	}else if(from == 1){
		new->first = tb->first;
		textnode *curr = tb->first;
		while(curr->line != to){
			curr = curr->next;
		}
		new->last = curr;
		tb->first = curr->next;
		tb->first->prev = NULL;
		updateline(tb);
		updateline(new);
	}else if(to == tb->row){
		textnode *curr = tb->first;
		while(curr->line != from){
			curr = curr->next;
		}
		new->first = curr;
		new->last = tb->last;
		tb->last = curr->prev;
		new->first->prev = NULL;
		updateline(tb);
		updateline(new);
	}else{
		textnode *curr = tb->first;
		while(curr->line != from){
			curr = curr->next;
		}
		textnode *tbprev = curr->prev;
		textnode *newhead = curr;
		while(curr->line != to){
			curr = curr->next;
		}
		textnode *newend = curr;
		textnode *tbback = curr->next;
		tbprev->next = tbback;
		tbback->prev = tbprev;
		new->first = newhead;
		new->first->prev = NULL;
		new->last = newend;
		newend->next = NULL;
		updateline(tb);
		updateline(new);
	}
	return new;
}



int strpos(char *source, char *search, int pos){
	// pos may start from 0
	if (strstr(source,search) == NULL) return -1;
	int i = pos;
	int sourcelen = strlen(source);
	int searchlen = strlen(search);
	int j;
    int check;
    int count;
	while(i < sourcelen){
		j = 0;
		if(source[i] == search[j]){
            check = i;
            count = 0;
			while(j < searchlen){
				if(source[i + j] == search[j]){
                    count++;
                }	
				j++;
			}
            if(count == searchlen) return check;
		}
		i++;
	}
    return -1;	
}
/**
 * Return  a  linked list of match nodes containing the positions of all
 * of the matches of string 'search' in 'tb'.
 * - The textbuffer 'tb' should remain unmodified.
 * - The user is responsible for freeing the returned list.
 */
Match searchTB(TB tb, char *search) {
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	if(search[0] == '\0') return NULL;
	if(search == NULL){
		fprintf(stderr,"searching string is NULL\n");
		abort();
	}
	int line = 0;
	Match head = NULL;
	textnode *curr = tb->first;
	while(curr != NULL){
		line = 0;
		while(line != -1){
			line = strpos(curr->string, search, line);
			if (line == -1) break;
			Match new = malloc(sizeof(*new));
			new->columnNumber = line;
			new->lineNumber = curr->line;
			if(head == NULL) {
	            head = new;
	        } else {
	            Match curr = head;
	            while (curr->next != NULL) {
		            curr = curr->next;
	            }
	            curr->next = new;
            }
		}
		curr = curr->next;
	}
	return head;
}

/**
 * Remove  the  lines between 'from' and 'to' (inclusive) from the given
 * textbuffer 'tb'.
 * - The  program should abort() with an error message if 'from' or 'to'
 *   is out of range.
 */
void deleteTB(TB tb, int from, int to) {
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
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
		updateline(tb);
	}else if(from == 1){
		tb->first = curr;
		updateline(tb);
	}else{
		newhead->next = curr;
		curr->prev = newhead;
		updateline(tb);
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
