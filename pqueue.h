/**
  @file pqueue.h
  @brief Priority queue implementation using heap

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdint.h>
#include <stdio.h>

/**
  Priority queue element structure.
  Contains element priority and payload.
*/
typedef struct {
    uint64_t priority;
    void *data;
} pqnode_t;

/**
  Priority queue structure.
  Contains info about queue allocated memory size, number of elements and pointer to elements' array.
*/
typedef struct {
    size_t size;      /**< number of elements */
    size_t maxsize;   /**< allocated memory size */
    pqnode_t *nodes;  /**< pointer to elements' array */
} pq_t;

/**
  @brief Create and initialize priority queue structure

  Allocates memory for the given number of elements
  @param[in] size size_t Initial size of the queue
  @return Pointer to new initialized queue
*/
pq_t* pq_init(size_t size);

/**
  @brief Destroy queue

  @param[in] pq pq ** Queue to destroy
*/
void pq_free(pq_t **pq);

/**
  @brief Push new item to the queue

  Finds place of new element and inserts it according to it's priority
  @param[in] pq pq_t * Queue to work with
  @param[in] data void * Pointer to the new element
  @param[in] pq uint64_t Priority of the new element
*/
void pq_push(pq_t *pq, void *data, uint64_t priority);

/**
  @brief Pop item with the lowest priority from the queue

  Pops the first element from the heap and reorganizes it
  @param[in] pq pq_t * Queue to work with
  @return Pointer to the popped element
*/
void* pq_pop(pq_t *pq);

#endif /* end of include guard: PQUEUE_H */
