#include "pqueue.h"
#include <stdlib.h>
#include "stdsafe.h"


pq_t* pq_init(size_t size) {
    pq_t *pq = (pq_t*)s_malloc(sizeof(pq_t));
    pq->maxsize = size;
    pq->size = 0;
    pq->nodes = (pqnode_t*)s_malloc(sizeof(pqnode_t) * (size + 1));
    return pq;
}

void pq_free(pq_t **pq) {
    free((*pq)->nodes);
    free(*pq);
    *pq = NULL;
}

void pq_swapNodes(pqnode_t *a, pqnode_t *b) {
    pqnode_t temp = *a;
    *a = *b;
    *b = temp;
}

void pq_push(pq_t *pq, void *data, uint64_t priority) {
    if (++(pq->size) == pq->maxsize) {
        pq->maxsize *= 2;
        pq->nodes = (pqnode_t*)s_realloc(pq->nodes, sizeof(pqnode_t) * (pq->maxsize + 1));
    }

    pq->nodes[pq->size].priority = priority;
    pq->nodes[pq->size].data = data;

    for (int i = pq->size; i > 1 && pq->nodes[i].priority <= pq->nodes[i / 2].priority; i = i / 2) {
        pq_swapNodes(&pq->nodes[i], &pq->nodes[i / 2]);
    }
}

void* pq_pop(pq_t *pq) {
    if (!pq->size) {
        return NULL;
    }
    pq_swapNodes(&pq->nodes[1], &pq->nodes[pq->size]);
    int j = 0;
    for (int k = 1, n = pq->size - 1; 2 * k <= n; k = j) {
        j = 2 * k;
        if (j < n && pq->nodes[j].priority >= pq->nodes[j + 1].priority) {
            j++;
        }
        if (pq->nodes[k].priority < pq->nodes[j].priority) {
            break;
        }
        pq_swapNodes(&pq->nodes[k], &pq->nodes[j]);
    }
    return pq->nodes[pq->size--].data;
}
