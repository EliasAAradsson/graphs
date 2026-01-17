#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    pq_t *pq = pq_create(10);

    char line[256];
    
    while (fgets(line, sizeof(line), stdin)) {
        // Remove newline if present
        line[strcspn(line, "\n")] = '\0';
        
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Find the two dashes
        char* first_dash = strchr(line, '-');
        if (!first_dash) continue;
        
        char* second_dash = strchr(first_dash + 1, '-');
        if (!second_dash) continue;

        size_t from_len = first_dash - line;
        char* fromNode = strndup(line, from_len);
        
        int cost = atoi(first_dash + 1);
        
        char* toNode = strdup(second_dash + 1);
        
        edge_t edge = (edge_t){.fromNode = fromNode, .cost = cost, .toNode = toNode};

        pq_push(pq, edge);
    }
    
    while (!pq_is_empty(pq)) {
        edge_t e = pq_pop(pq);
        printf("Cost: %d\n", e.cost);
    }
    
    pq_destroy(pq);
    return 0;
}
