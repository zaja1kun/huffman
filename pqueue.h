#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdint.h>
#include <stdio.h>


typedef struct {
    uint64_t priority;
    void *data;
} pqnode_t;

typedef struct {
    size_t size;
    size_t maxsize;
    pqnode_t *nodes;
} pq_t;


pq_t* pq_init(size_t size);
void pq_free(pq_t **pq);
void pq_push(pq_t *pq, void *data, uint64_t priority);
void* pq_pop(pq_t *pq);

#endif /* end of include guard: PQUEUE_H */
