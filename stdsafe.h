#ifndef STDSAFE_H
#define STDSAFE_H

#include <stdio.h>
#include <stdint.h>

void* s_malloc(size_t size);
void *s_calloc(size_t nmemb, size_t size);
void *s_realloc(void *ptr, unsigned size);
void s_free(void **ptr);
FILE* s_fopen(const char *filename, const char *mode);
void s_exit(int code);
uint64_t getFileSize(FILE * const file);

#endif /* end of include guard: STDSAFE_H */
