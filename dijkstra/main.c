#include "heap.h"

#include <stdio.h>

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
