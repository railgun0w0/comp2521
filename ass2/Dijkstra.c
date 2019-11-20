//Dijkstra.c
// made by yanghaoyu for comp2521 ass2
#include "Dijkstra.h"
#include "PQ.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define INFINITY 10000
ShortestPaths dijkstra(Graph g, Vertex src);
void showShortestPaths(ShortestPaths sps);
void freeShortestPaths(ShortestPaths sps);

ShortestPaths dijkstra(Graph g, Vertex src){
    // Create a new empty ShortestPaths structure 
    assert(g != NULL);
    ShortestPaths new;
    new.numNodes = GraphNumVertices(g);
    new.src = src;
    new.pred = malloc(new.numNodes * sizeof(PredNode *));
    new.dist = malloc(new.numNodes * sizeof(int));
    PQ quene = PQNew();
    
    for(int i = 0; i < new.numNodes; i++) {
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
    PQUpdate(quene,srcitem);
    // init complete
    while(!PQIsEmpty(quene)){
        ItemPQ curr = PQDequeue(quene);
        // curr is start point

        AdjList edgelist = GraphOutIncident(g,curr.key);
        while(edgelist != NULL){
            PredNode *node = malloc(sizeof(PredNode));
            node->v = curr.key;
            node->next = NULL;
            int length = edgelist->weight + new.dist[curr.key];
            if (length == new.dist[edgelist->v]){
                if(new.pred[edgelist->v] == NULL) {
                    new.pred[edgelist->v] = node;
                }else{
                    PredNode *currnode = new.pred[edgelist->v];
                    while(currnode->next != NULL){
                        currnode = currnode->next;
                    }
                    currnode->next = node;
                }
            }else if(length < new.dist[edgelist->v]) {
                new.dist[edgelist->v] = length;
                if(new.pred[edgelist->v] == NULL) {
                    new.pred[edgelist->v] = node;
                }else{
                    PredNode *prev = new.pred[edgelist->v];
                    while(prev != NULL){
                        PredNode *now = prev->next;
                        free(prev);
                        prev = now;
                    }
                    new.pred[edgelist->v] = node;
                }
                ItemPQ newitem;
                newitem.key = edgelist->v;
                newitem.value = length;
                PQUpdate(quene,newitem);
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

    return;
}

void freeShortestPaths(ShortestPaths sps){
    free(sps.dist);
    for(int i = 0; i < sps.numNodes; i++) {
        if(sps.pred[i] == NULL){
            continue;
        }else{
            PredNode *prev = sps.pred[i];
            while(prev != NULL){
                PredNode *next = prev->next;
                free(prev);
                prev = next;
            }
            sps.pred[i] = NULL;     
        }
	}
    free(sps.pred);
    //free(sps);
}