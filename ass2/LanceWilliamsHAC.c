// LanceWilliamsHAC.c
// made by yanghaoyu(z5223796) for comp2521 ass2
#include "LanceWilliamsHAC.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF -1

Dendrogram LanceWilliamsHAC (Graph g, int method);
void freeDendrogram(Dendrogram d);

// helper function
static double single_lance_williams(double j, double k);
static double weightvertices(Graph g, Vertex src, Vertex dest);
static int is_leaf(Dendrogram head, Vertex aim);
static double complete_lance_williams(double ik, double jk);

Dendrogram LanceWilliamsHAC (Graph g, int method) {
    int num = GraphNumVertices(g);
    double **array = (double **) malloc(sizeof(double *) * num);
    for (int i = 0; i < num; i++) {
        array[i] = (double *) malloc(sizeof(double) * num);
    }
    for (int x = 0; x < num; x++ ) {
        for (int y = 0; y < num; y++) {
            if (x == y) continue;
            if (x > y) continue;
            if (GraphIsAdjacent(g, x, y)  == true || GraphIsAdjacent(g, y, x) == true) {
                if (GraphIsAdjacent(g, x, y) == false && weightvertices(g, y, x) == true) {
                    array[x][y] = (double) 1.0 / weightvertices(g, y, x);
                } else if (GraphIsAdjacent(g, y, x) == false && weightvertices(g, x, y) == true) {
                    array[x][y] = (double) 1.0 / weightvertices(g, x, y);
                } else if (weightvertices(g, x , y) >= weightvertices(g, y, x)) {
                    array[x][y] = (double) 1.0 / weightvertices(g, x, y);
                } else {
                    array[x][y] = (double) 1.0 / weightvertices(g, y, x);
                }
            } else {
                array[x][y] = (double) INF;
            }
        }
    }
    Dendrogram *new = malloc(num  * sizeof(Dendrogram));
    for (int i = 0; i < num; i++) {
        new[i] = malloc(sizeof(DNode));
        new[i]->vertex = i;
        new[i]->left = NULL;
        new[i]->right = NULL;
    }
    // init complete
    int returnx, returny;
    while (1) {
        double min = __INT_MAX__;
        double max = 0;
        for (int x = 0; x < num; x++ ) {
            for (int y = 0; y < num; y++) {
                if (x == y) continue;
                if (x > y) continue;
                if (array[x][y] == INF) {
                    continue;
                }
                if (method == 1) {
                    if (array[x][y] < min) {
                        min = array[x][y];
                        returnx = x;
                        returny = y;
                    }
                } else {
                    if (array[x][y] > max) {
                        max = array[x][y];
                        returnx = x;
                        returny = y;
                    }
                }
            }
        }
        Dendrogram newnode = malloc(sizeof(DNode));
        newnode->vertex = -1;
        newnode->left = NULL;
        newnode->right = NULL;
        for (int i = 0; i < num; i++) {
            if (new[i] == NULL) continue;
            double value = new[i]->vertex;
            if (value == -1) continue;
            if (value == returnx) {
                newnode->left = new[i];
                new[i] = NULL;
            }
            if (value == returny) {
                newnode->right = new[i];
                new[i] = NULL;
            }
        }
        if (newnode->right == NULL) {
            for (int i = 0; i < num; i++) {
                if (new[i] == NULL) continue;
                if (is_leaf(new[i], returny) == 1) {
                    newnode->right = new[i];
                    new[i] = NULL;
                }
            }
        }
        if (newnode->left == NULL) {
            for (int i = 0; i < num; i++) {
                if (new[i] == NULL) continue;
                if (is_leaf(new[i], returnx) != 0) {
                    newnode->left = new[i];
                    new[i] = NULL;
                }
            }
        }
        int neworder = 0;
        for (int i = 0; i < num; i++) {
            if (new[i] == NULL) {
                continue;
            } else {
                new[neworder] = new[i];
                if (i != neworder) new[i] = NULL;
                neworder++;
            }
        }
        new[neworder] = newnode;
        if (new[0]->vertex == -1 && neworder == 0 ) {
            break;
        }
        
        //Update the DIST array
        for (int k = 0; k < num; k++) {
            for (int kk = 0; kk < num; kk++) {
                if (kk == k) continue;
                if (k > kk) continue;
                if ((k != returnx) && (k != returny) && (array[k] != NULL)) {
                    if (method == 1) {
                        array[k][returnx] = single_lance_williams(array[k][returnx], array[k][returny]);
                    } else {
                        array[k][returnx] = complete_lance_williams(array[k][returnx], array[k][returny]);
                    }
                }
                array[k][returny] = (double) INF;
                array[returny][k] = (double) INF;
            }
        }
    }
    for (int i = 0;i < num;i++)
        free(array[i]);
    free(array);
    return new[0];
}

static double single_lance_williams(double ik, double jk){
    if (ik == INF) {
        return jk;
    }
    if (jk == INF) {
        return ik;
    }
    double result = 0.0;
    double absvalue = (ik - jk);
    if (absvalue < 0) absvalue = -1.0 * absvalue;
    result = (0.5 * ik + 0.5 * jk - 0.5 * absvalue);
    return result;
}

static double complete_lance_williams(double ik, double jk){
    if (ik == INF) {
        return jk;
    }
    if (jk == INF) {
        return ik;
    }
    double result = 0.0;
    double absvalue = (ik - jk);
    if (absvalue < 0) absvalue = -1.0 * absvalue;
    result = (0.5 * ik + 0.5 * jk + 0.5 * absvalue);
    return result;
}

static double weightvertices(Graph g, Vertex src, Vertex dest){
    AdjList checklist = GraphInIncident(g, src);
    while (checklist != NULL) {
        if (checklist->v == dest) return (double) checklist->weight;
        checklist = checklist->next;
    }
    return 0;//keep happy
}
void freeDendrogram(Dendrogram d){
    return;
}

static int is_leaf(Dendrogram head, Vertex aim){
    if (head == NULL ) return 0;
    while (head->vertex != aim) {
        return is_leaf(head->left, aim) + is_leaf(head->right, aim);
    }
    return 1;
}