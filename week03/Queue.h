// Queue.h ... interface to Queue-of-BSTree-nodes ADT

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "BSTree.h"

typedef struct BSTNode *Item;

#define showItem showBSTreeNode

typedef struct QueueRep *Queue;

// create new empty queue
Queue newQueue (void);
// free memory used by queue
void dropQueue (Queue);
// display as 3 > 5 > 4 > ...
void showQueue (Queue);
// add item on queue
void QueueJoin (Queue, Item);
// remove item from queue
Item QueueLeave (Queue);
// check for no items
bool QueueIsEmpty (Queue);

#endif
