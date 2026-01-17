typedef struct {
    char* fromNode;
    int cost;
    char* toNode;
} edge_t;

typedef struct {
    edge_t *edges;
    int size;
    int capacity;
} pq_t;

/**
 * Create a new priority queue
 */
pq_t* pq_create(int capacity);

/**
 * Insert an edge into the priority queue
 */
void pq_push(pq_t *pq, edge_t edge);

/**
 * Remove and return the edge with minimum cost
 */
edge_t pq_pop(pq_t *pq);

/**
 * Peek at the minimum edge without removing it
 */
edge_t pq_peek(pq_t *pq);

/**
 * Check if the queue is empty
 */
int pq_is_empty(pq_t *pq);

/**
 * Get the size of the queue
 */
int pq_size(pq_t *pq);

/**
 * Free the priority queue
 */
void pq_destroy(pq_t *pq);
