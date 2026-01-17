#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int cost;
} edge_t;

typedef struct {
    edge_t *edges;
    int size;
    int capacity;
} pq_t;

/**
 * Create a new priority queue
 */
pq_t* pq_create(int capacity) {
    pq_t *pq = malloc(sizeof(pq_t));
    pq->edges = malloc(sizeof(edge_t) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

/**
 * Helper functions for heap operations
 */
static void swap(edge_t *a, edge_t *b) {
    edge_t temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify_up(pq_t *pq, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (pq->edges[idx].cost < pq->edges[parent].cost) {
            swap(&pq->edges[idx], &pq->edges[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

static void heapify_down(pq_t *pq, int idx) {
    while (1) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < pq->size && pq->edges[left].cost < pq->edges[smallest].cost) {
            smallest = left;
        }
        if (right < pq->size && pq->edges[right].cost < pq->edges[smallest].cost) {
            smallest = right;
        }
        
        if (smallest != idx) {
            swap(&pq->edges[idx], &pq->edges[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}

/**
 * Insert an edge into the priority queue
 */
void pq_push(pq_t *pq, edge_t edge) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->edges = realloc(pq->edges, sizeof(edge_t) * pq->capacity);
    }
    
    pq->edges[pq->size] = edge;
    heapify_up(pq, pq->size);
    pq->size++;
}

/**
 * Remove and return the edge with minimum cost
 */
edge_t pq_pop(pq_t *pq) {
    edge_t min_edge = pq->edges[0];
    pq->size--;
    pq->edges[0] = pq->edges[pq->size];
    heapify_down(pq, 0);
    return min_edge;
}

/**
 * Peek at the minimum edge without removing it
 */
edge_t pq_peek(pq_t *pq) {
    return pq->edges[0];
}

/**
 * Check if the queue is empty
 */
int pq_is_empty(pq_t *pq) {
    return pq->size == 0;
}

/**
 * Get the size of the queue
 */
int pq_size(pq_t *pq) {
    return pq->size;
}

 
/**
 * Free the priority queue
 */
void pq_destroy(pq_t *pq) {
    free(pq->edges);
    free(pq);
}

// Example
int main() {
    pq_t *pq = pq_create(10);
    
    pq_push(pq, (edge_t){.cost = 5});
    pq_push(pq, (edge_t){.cost = 2});
    pq_push(pq, (edge_t){.cost = 8});
    pq_push(pq, (edge_t){.cost = 1});
    
    while (!pq_is_empty(pq)) {
        edge_t e = pq_pop(pq);
        printf("Cost: %d\n", e.cost);
    }
    
    pq_destroy(pq);
    return 0;
}
