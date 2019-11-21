#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdio.h>
#include <stdlib.h>

static double totalshortestpath(ShortestPaths path, Vertex dest){
    if(path.pred[dest] == NULL) return 1;
    double num = 0;
    PredNode *curr = path.pred[dest];
    while(curr != NULL){
        num = num + totalshortestpath(path, curr->v);
        curr = curr->next;
    }
    return num;
}
static double totalpassthroughpath(ShortestPaths path, Vertex dest, Vertex mark){
    double time = 0;
    PredNode *curr = path.pred[dest];
    while(curr != NULL){
        if(curr->v == mark) time += totalshortestpath(path,mark);
        time += totalpassthroughpath(path, curr->v, mark);
        curr = curr->next;
    }
    return time;
}

NodeValues closenessCentrality(Graph g){
    NodeValues closenessresult;
    closenessresult.numNodes = GraphNumVertices(g);
    closenessresult.values = malloc(sizeof(closenessresult.values) * closenessresult.numNodes);
    for(int u = 0; u < closenessresult.numNodes; u++){
        double n = 0;
        double sumpath = 0;
        // caluate n
        ShortestPaths path = dijkstra(g,u);
        for(int i = 0; i < path.numNodes; i++){
            PredNode *curr = path.pred[i];
            if(curr != NULL) n++;
            sumpath += path.dist[i];
        }
        n = n + 1;
        double N = GraphNumVertices(g);
        if(sumpath != 0){
            double result = ((n-1)/(N-1))*((n-1)/(sumpath));
            closenessresult.values[u] = result;
        }else{
            closenessresult.values[u] = 0;
        }
    }
    return closenessresult;
}

NodeValues betweennessCentrality(Graph g){
    NodeValues betweennessresult;
    betweennessresult.numNodes = GraphNumVertices(g);
    betweennessresult.values = malloc(sizeof(betweennessresult.values) * betweennessresult.numNodes);
    for(int i = 0; i < betweennessresult.numNodes; i++) {
        betweennessresult.values[i] = 0;
    }
    for (int s = 0; s < betweennessresult.numNodes; s++) {
        ShortestPaths paths = dijkstra(g,s);
        for(int t = 0;t < betweennessresult.numNodes; t++){
            if (s == t) continue;
            for(int v = 0; v < betweennessresult.numNodes; v++){
                if(v == s || v == t) continue;
                // calaute
                double totalshortpaths = totalshortestpath(paths, t);
                double totalpassthrough = totalpassthroughpath(paths, t, v);
                if(paths.pred[t] == NULL) totalshortpaths = 0;
                if(totalshortpaths != 0){
                    betweennessresult.values[v] += totalpassthrough/totalshortpaths;
                }
            }
        }
    }
    return betweennessresult;
}

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues norbet = betweennessCentrality(g);
    double n = norbet.numNodes;
    for(int i = 0; i < n; i++){
        norbet.values[i] = norbet.values[i]*(1/((n-1)*(n-2)));
    }
    return norbet;
}

void showNodeValues(NodeValues nvs){
    int i = 0;
    while(i < nvs.numNodes){
        printf("%d: %f\n",i,nvs.values[i]);
        i++;
    }
}

void freeNodeValues(NodeValues nvs){
    free(nvs.values);
}