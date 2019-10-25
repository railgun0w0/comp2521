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

static void updateline(TB tb);

static TB emptyTB (void) {
	TB new = malloc(sizeof *new);
	if (new == NULL) printf("couldn't allocate textbuffer");
	new->row = 0;
	new->first = new->last = NULL;
	return new;
}

static textnode *newnode (char *string) {
	textnode *newTN = malloc(sizeof *newTN);
	if (newTN == NULL) printf("couldn't allocate textnode");
	newTN->next = newTN->prev = NULL;
	newTN->string = string;
	newTN->line = 0;
	return newTN;
}

static void insertnode(TB T, textnode *node) {
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

static void copycopycopy(char *source, char *really, int prev, int goal) {
	int i = 0;
	while(prev < goal){
		really[i] = source[prev];
		i++;
		prev++;
	}
	really[i] = '\n';
	really[i+1] = '\0';
}

static void tokenise(char *text, TB T) {
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
		free(curr->string);
		free(curr);
		curr = next;
	}
	free (tb);
}

/**
 * Allocate  and return a string containing all of the text in the given
 * textbuffer. If showLineNumbers is true, add a prefix corresponding to
 * the line number.
 */

int countsize(TB tb) {
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
		str = malloc(sizeof(char)*strsiz + 1);
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
    if(to < from || from < 1 || to > tb->row){
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
	if(pos <= 0 || pos > tb1->row+1){
		fprintf(stderr,"out of range\n");
		abort();
	}
	if(tb1 == tb2) return;
	if(tb1->first == NULL && tb2->first == NULL){
		free(tb2);
		return;
	}else if(tb1->first == NULL && pos == 1){
		tb1->first = tb2->first;
		tb1->last = tb2->last;
		tb1->row = tb2->row;
		tb2->first = tb2->last = NULL;
		tb2->row = 0;
		free(tb2);
		return;
	}else if(tb2->first == NULL){
		free(tb2);
		return;
	}else{
		if(pos == (tb1->row + 1)){
			tb1->last->next = tb2->first;
			tb2->first->prev = tb1->last;
			tb2->first = NULL;
			tb1->last = tb2->last;
			tb2->last = NULL;
			tb2->row = 0;
			updateline(tb1);
			free(tb2);
			return;
		}else if(pos == 1){
			tb1->first->prev = tb2->last;
			tb2->last->next = tb1->first;
			tb1->first = tb2->first;
			tb2->first = NULL;
			tb2->last = NULL;
			tb2->row = 0;
			updateline(tb1);
			free(tb2);
			return;
		}else{
			textnode *tb1curr = tb1->first;
			while(tb1curr->line != pos){
				tb1curr = tb1curr->next;
			}
			//         A b o - oC D 
			// o - o - o\ / o - o -
			textnode *A = tb1curr->prev;
			textnode *B = tb2->first;
			textnode *C = tb2->last;
			textnode *D = tb1curr;
			//connect
			A->next = B;
			B->prev = A;
			C->next = D;
			D->prev = C;
			updateline(tb1);
			tb2->last = tb2->first = NULL;
			tb2->row = 0;
			free(tb2);
			return;
		}
	}
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
    if(pos <= 0 || pos > tb1->row+1){
		fprintf(stderr,"out of range\n");
		abort();
	}
	if(tb1->first == NULL && tb2->first == NULL){
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
	}else if(tb2->first == NULL){
		return;
	}else{
		if(pos == 1){
			textnode *C = tb1->first;
			textnode *D = tb1->last;
			textnode *tb2curr = tb2->first;
			while(tb2curr != NULL){
				char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
				string[0] = '\0';
				strcpy(string,tb2curr->string);
				textnode *new = newnode(string);
				insertnode(tb1,new);
				tb2curr = tb2curr->next;
			}
			textnode *B = tb1->last;
			textnode *A = D->next;
			tb1->first = A;
			A->prev = NULL;
			tb1->last = D;
			D->next = NULL;
			B->next = C;
			C->prev = B;
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
			textnode *tb2curr = tb2->first;
			textnode *A = tb1curr->prev;
			textnode *D = tb1curr;
			textnode *tmp = tb1->last;
			while(tb2curr != NULL){
				char *string = malloc(sizeof(char)*strlen(tb2curr->string)+1);
				string[0] = '\0';
				strcpy(string,tb2curr->string);
				textnode *new = newnode(string);
				insertnode(tb1,new);
				tb2curr = tb2curr->next;	
			}
			textnode *B = tmp->next;
			textnode *C = tb1->last;
			A->next = B;
			B->prev = A;
			C->next = D;
			D->prev = C;
			tb1->last = tmp;
			tb1->last->next = NULL;
			updateline(tb1);
			return;
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
		curr->next = NULL;
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
		tb->last->next = NULL;
		new->first->prev = NULL;
		updateline(tb);
		printf("tb pass");
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
            if(count == searchlen) return check + 1;
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
	int line;
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
			line = line + strlen(search);
			new->next = NULL;
			if(head == NULL) {
	            head = new;
	        } else {
	            Match mtcurr = head;
	            while (mtcurr->next != NULL) {
		            mtcurr = mtcurr->next;
	            }
	            mtcurr->next = new;
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


void replace(textnode *node,char *string){
	int count_star = 0;
	int count_under = 0;
	int i = 0;
	int strsiz = strlen(string);
	int newstrsize = 0;
	int starpro = 0;
	int underpro = 0;
	int usestar = 0;
	int useunder = 0;
	int starsearch;
	int undersearch;
	if(string[0] == '#' && strlen(string) <= 2) return;
	if(string[0] == '#' && string[1] != '\0'){
		char *newstring = malloc(sizeof(char)*strsiz + 9 + 1);
		newstring[0] = '\0';
		strcat(newstring,"<h1>");
		i = i + 4;
		int move = 1;
		while(string[move] != '\n'){
			newstring[i] = string[move];
			move++;
			i++;
		}
		newstring[i] = '\0';
		strcat(newstring,"</h1>");
		newstring[i + 5] = '\n';
		newstring[i + 6] = '\0';
		free(node->string);
		node->string = newstring;
		return;
	}
	while(i < strsiz){
		if(string[i] == '*' && string[i + 1] != '*'){
			starsearch = i + 1;
			while(starsearch < strsiz){
				if(string[starsearch] == '*'){
					count_star = count_star + 2;
					starpro = 1;
					break;
				}
				starsearch++;
			}
			if(starpro == 1){
				i = starsearch + 1;
			}
		}
		if(string[i] == '_' && string[i + 1] != '_'){
			undersearch = i + 1;
			while(undersearch < strsiz){
				if(string[undersearch] == '_'){
					count_under = count_under + 2;
					underpro = 1;
					break;
				}
				undersearch++;
			}
			i = undersearch + 1;
		}
		i++;
	}
	if(starpro != 0){
		if(count_star %2 == 0 ){
			newstrsize += (count_star/2)*7;
			usestar = count_star;
		}
	}
	if(underpro != 0){
		if(count_under %2 == 0 ){
			newstrsize += (count_under/2)*7;
			useunder = count_under;
		}
	}
	newstrsize += strsiz;
	char *newstring = malloc(sizeof(char)* newstrsize + 1);
	newstring[0] = '\0';
	i = 0;
	int move = 0;
	int numstar = 0;
	int numunder = 0;
	while(move < strsiz){
		newstring[i] = string[move];
		if(starpro == 1){
			if(string[move] == '*' && string[move + 1] != '*'){
				if(numstar < usestar){
					newstring[i] = '\0';
					strcat(newstring,"<b>");
					i = i + 3;
					int search = move + 1;
					while(string[search]!= '*'){
						search++;
					}
					//printf("search pos : %d",search);
					//printf("i value : %d",i);
					move++;
					//printf("move value: %d", move);
					while(move < search){
						newstring[i] = string[move];
						i++;
						move++;
					}
					newstring[i] = '\0';
					strcat(newstring,"</b>");
					i = i + 3;
					numstar = numstar + 2;
				}
			}
		}
		if(underpro == 1){
			if(string[move] == '_' && string[move + 1] != '_'){
				if(numunder < useunder){
					newstring[i] = '\0';
					strcat(newstring,"<i>");
					i = i + 3;
					int search = move + 1;
					while(string[search]!= '_'){
						search++;
					}
					//printf("search pos : %d",search);
					//printf("i value : %d",i);
					move++;
					//printf("move value: %d", move);
					while(move < search){
						newstring[i] = string[move];
						i++;
						move++;
					}
					newstring[i] = '\0';
					strcat(newstring,"</i>");
					i = i + 3;
					numunder = numunder + 2;
				}
			}
		}
		move++;
		i++;
	}
	newstring[i] = '\0';
	free(node->string);
	node->string = newstring;
}
/**
 * Search  every  line of the given textbuffer for every occurrence of a
 * set of specified substitutions and alter them accordingly.
 * - Refer to the spec for details.
 */
void formRichText(TB tb) {
	if (tb == NULL){
		fprintf(stderr,"tb is null\n");
		abort();
	}
	textnode *new = tb->first;
	while(new != NULL){
		replace(new,new->string);
		new = new->next;
	}
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
