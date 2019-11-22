// Dijkstra.c
// made by yanghaoyu(z5223796) for comp2521 ass2
#include "Dijkstra.h"
#include "PQ.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define INFINITY 10000

ShortestPaths dijkstra(Graph g, Vertex src);
void showShortestPaths(ShortestPaths sps);
void freeShortestPaths(ShortestPaths sps);

// helper function
static void insectnode(ShortestPaths sps, Vertex value, Vertex dest);
static void freelist(PredNode *node);

ShortestPaths dijkstra(Graph g, Vertex src){
    // Create a new empty ShortestPaths structure 
    assert(g != NULL);
    ShortestPaths new;
    new.numNodes = GraphNumVertices(g);
    new.src = src;
    new.pred = malloc(new.numNodes * sizeof(PredNode *));
    new.dist = malloc(new.numNodes * sizeof(int));
    PQ quene = PQNew();
    
    for (int i = 0; i < new.numNodes; i++) {
		new.dist[i] = INFINITY;
		new.pred[i] = NULL;
        ItemPQ item;
        item.key = i;
        item.value = new.dist[i];
        PQAdd(quene, item);

	}
	new.dist[src] = 0;
	ItemPQ srcitem;
    srcitem.key = src;
    srcitem.value = 0;
    PQUpdate(quene, srcitem);
    // init complete
    while (!PQIsEmpty(quene)) {
        ItemPQ curr = PQDequeue(quene);
        // curr is start point
        AdjList edgelist = GraphOutIncident(g, curr.key);
        while (edgelist != NULL) {
            int length = edgelist->weight + new.dist[curr.key];
            if (length == new.dist[edgelist->v]) {
                insectnode(new, curr.key, edgelist->v);
            } else if (length < new.dist[edgelist->v]) {
                new.dist[edgelist->v] = length;
                freelist(new.pred[edgelist->v]);
                new.pred[edgelist->v] = NULL;
                insectnode(new, curr.key, edgelist->v);
                // update quene and increase priority
                ItemPQ newitem;
                newitem.key = edgelist->v;
                newitem.value = length;
                PQUpdate(quene, newitem);
            }
            edgelist = edgelist->next;
        }
    }
    for (int i = 0; i < new.numNodes; i++) {
		if (new.dist[i] == INFINITY) {
			new.dist[i] = 0;
			new.pred[i] = NULL;
		}
	}
    PQFree(quene);
    return new;
}

void showShortestPaths(ShortestPaths sps){
    for(int i = 0; i < sps.numNodes;i++) {
        PredNode *curr = sps.pred[i];
        while (curr != NULL) {
            printf("pred[%d] = %d\n",i,curr->v);
            curr = curr->next;
        }
    }
    return;
}

void freeShortestPaths(ShortestPaths sps){
    free(sps.dist);
    for (int i = 0; i < sps.numNodes; i++) {
        if (sps.pred[i] == NULL) {
            continue;
        } else {
            PredNode *prev = sps.pred[i];
            while (prev != NULL) {
                PredNode *next = prev->next;
                free(prev);
                prev = next;
            }
            sps.pred[i] = NULL;     
        }
	}
    free(sps.pred);
}

// helper function 
static void insectnode(ShortestPaths sps, Vertex value, Vertex dest){
    PredNode *node = malloc(sizeof(PredNode));
    node->v = value;
    node->next = NULL;
    if (sps.pred[dest] == NULL) {
        sps.pred[dest] = node;
    } else {
        PredNode *currnode = sps.pred[dest];
        while (currnode->next != NULL) {
            currnode = currnode->next;
        }
        currnode->next = node;
    }
}
static void freelist(PredNode *node){
    PredNode *prev = node;
    while (prev != NULL) {
        PredNode *now = prev->next;
        free(prev);
        prev = now;
    }
}