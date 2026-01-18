#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50

// Structure to represent a node in adjacency list
typedef struct AdjListNode {
    int dest;       // Index of destination vertex
    int weight;
    struct AdjListNode* next;
} AdjListNode;

// Structure to represent an adjacency list
typedef struct AdjList {
    AdjListNode* head;
} AdjList;

// Structure to represent a graph
typedef struct Graph {
    int V;
    char** names;   // Array of vertex names
    AdjList* array;
} Graph;

// Structure to represent a min heap node
typedef struct MinHeapNode {
    int v;
    int dist;
} MinHeapNode;

// Structure to represent a min heap
typedef struct MinHeap {
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

// Create a new adjacency list node
AdjListNode* newAdjListNode(int dest, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Create a graph with V vertices
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    graph->names = (char**)malloc(V * sizeof(char*));
    
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
        graph->names[i] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
    }
    
    return graph;
}

// Set name for a vertex
void setVertexName(Graph* graph, int v, const char* name) {
    if (v >= 0 && v < graph->V) {
        strncpy(graph->names[v], name, MAX_NAME_LEN - 1);
        graph->names[v][MAX_NAME_LEN - 1] = '\0';
    }
}

// Find vertex index by name
int findVertexByName(Graph* graph, const char* name) {
    for (int i = 0; i < graph->V; i++) {
        if (strcmp(graph->names[i], name) == 0) {
            return i;
        }
    }
    return -1;  // Not found
}

// Add an edge to undirected graph using vertex names
void addEdgeByName(Graph* graph, const char* src, const char* dest, int weight) {
    int srcIdx = findVertexByName(graph, src);
    int destIdx = findVertexByName(graph, dest);
    
    if (srcIdx == -1 || destIdx == -1) {
        printf("Error: Vertex not found\n");
        return;
    }
    
    // Add edge from src to dest
    AdjListNode* newNode = newAdjListNode(destIdx, weight);
    newNode->next = graph->array[srcIdx].head;
    graph->array[srcIdx].head = newNode;
    
    // Add edge from dest to src (undirected graph)
    newNode = newAdjListNode(srcIdx, weight);
    newNode->next = graph->array[destIdx].head;
    graph->array[destIdx].head = newNode;
}

// Add an edge using indices
void addEdge(Graph* graph, int src, int dest, int weight) {
    // Add edge from src to dest
    AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    
    // Add edge from dest to src
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Create a min heap node
MinHeapNode* newMinHeapNode(int v, int dist) {
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// Create a min heap
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// Swap two nodes in min heap
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// Heapify at given index
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < minHeap->size && 
        minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;
    
    if (right < minHeap->size && 
        minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;
    
    if (smallest != idx) {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];
        
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        
        minHeapify(minHeap, smallest);
    }
}

// Check if heap is empty
int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

// Extract minimum node from heap
MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;
    
    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;
    
    --minHeap->size;
    minHeapify(minHeap, 0);
    
    return root;
}

// Decrease distance value of a given vertex
void decreaseKey(MinHeap* minHeap, int v, int dist) {
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        
        i = (i - 1) / 2;
    }
}

// Check if a vertex is in min heap
bool isInMinHeap(MinHeap* minHeap, int v) {
    return minHeap->pos[v] < minHeap->size;
}

// Print solution with vertex names
void printArr(Graph* graph, int dist[], int n) {
    printf("Vertex\t\tDistance from Source\n");
    for (int i = 0; i < n; ++i) {
        if (dist[i] == INT_MAX)
            printf("%-15s\tINF\n", graph->names[i]);
        else
            printf("%-15s\t%d\n", graph->names[i], dist[i]);
    }
}

// Dijkstra's algorithm
void dijkstra(Graph* graph, const char* srcName) {
    int src = findVertexByName(graph, srcName);
    if (src == -1) {
        printf("Error: Source vertex '%s' not found\n", srcName);
        return;
    }
    
    int V = graph->V;
    int dist[V];
    
    MinHeap* minHeap = createMinHeap(V);
    
    for (int v = 0; v < V; ++v) {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    
    minHeap->size = V;
    
    while (!isEmpty(minHeap)) {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        
        AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->dest;
            
            if (isInMinHeap(minHeap, v) && 
                dist[u] != INT_MAX && 
                pCrawl->weight + dist[u] < dist[v]) {
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    
    printf("\nShortest paths from '%s':\n", srcName);
    printArr(graph, dist, V);
}

// Free graph memory
void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {
        AdjListNode* current = graph->array[i].head;
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
        free(graph->names[i]);
    }
    free(graph->names);
    free(graph->array);
    free(graph);
}

// Example usage
int main() {
    int V = 9;
    Graph* graph = createGraph(V);
    
    // Set vertex names
    setVertexName(graph, 0, "New York");
    setVertexName(graph, 1, "Boston");
    setVertexName(graph, 2, "Philadelphia");
    setVertexName(graph, 3, "Washington");
    setVertexName(graph, 4, "Atlanta");
    setVertexName(graph, 5, "Chicago");
    setVertexName(graph, 6, "Detroit");
    setVertexName(graph, 7, "Cleveland");
    setVertexName(graph, 8, "Pittsburgh");
    
    // Add edges using names
    addEdgeByName(graph, "New York", "Boston", 4);
    addEdgeByName(graph, "New York", "Cleveland", 8);
    addEdgeByName(graph, "Boston", "Philadelphia", 8);
    addEdgeByName(graph, "Boston", "Cleveland", 11);
    addEdgeByName(graph, "Philadelphia", "Washington", 7);
    addEdgeByName(graph, "Philadelphia", "Pittsburgh", 2);
    addEdgeByName(graph, "Philadelphia", "Chicago", 4);
    addEdgeByName(graph, "Washington", "Atlanta", 9);
    addEdgeByName(graph, "Washington", "Chicago", 14);
    addEdgeByName(graph, "Atlanta", "Chicago", 10);
    addEdgeByName(graph, "Chicago", "Detroit", 2);
    addEdgeByName(graph, "Detroit", "Cleveland", 1);
    addEdgeByName(graph, "Detroit", "Pittsburgh", 6);
    addEdgeByName(graph, "Cleveland", "Pittsburgh", 7);
    
    // Run Dijkstra from "New York"
    dijkstra(graph, "Detroit");
    
    // Clean up
    freeGraph(graph);
    
    return 0;
}
