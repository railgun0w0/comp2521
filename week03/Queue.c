// Queue.h ... implementation of Queue ADT
// assumes that Item is an assignable type
// (e.g. int, pointer) defined in Queue.h

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "Queue.h"

typedef struct QueueNode {
	Item value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head; // ptr to first node
	QueueNode *tail; // ptr to last node
} QueueRep;

// create new empty Queue
Queue newQueue (void)
{
	Queue new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Queue");
	*new = (QueueRep) { .head = NULL, .tail = NULL };
	return new;
}

// free memory used by Queue
void dropQueue (Queue Q)
{
	assert (Q != NULL);

	// free list nodes
	QueueNode *curr = Q->head;
	while (curr != NULL) {
		QueueNode *next = curr->next;
		free (curr);
		curr = next;
	}
	// free queue rep
	free (Q);
}

// display as 3 > 5 > 4 > ...
void showQueue (Queue Q)
{
	assert (Q != NULL);

	for (QueueNode *curr = Q->head; curr != NULL; curr = curr->next) {
		showItem (curr->value);
		if (curr->next != NULL)
			printf (" > ");
	}
}

// add item at end of Queue
void QueueJoin (Queue Q, Item it)
{
	assert (Q != NULL);

	QueueNode *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Queue node");
	*new = (QueueNode) { .value = it, .next = NULL };

	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;
}

// remove item from front of Queue
Item QueueLeave (Queue Q)
{
	assert (Q != NULL);
	assert (Q->head != NULL);
	Item it = Q->head->value;
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL)
		Q->tail = NULL;
	free (old);
	return it;
}

// check for no items
bool QueueIsEmpty (Queue Q)
{
	return Q->head == NULL;
}
