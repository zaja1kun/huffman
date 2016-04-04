/**
  @file stdsafe.h
  @brief Safe versions of standard functions for memory allocation, operations with files, etc.

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#ifndef STDSAFE_H
#define STDSAFE_H

#include <stdio.h>
#include <stdint.h>

#define FILESIZE_T uint64_t

/**
  @brief Safe version of malloc

  Allocates memory aligned to standard cache line size.
  Terminates application in case of any errors.
  @param[in] size size_t Size of memory to allocate
  @return Pointer to the allocated memory
*/
void* s_malloc(size_t size);

/**
  @brief Safe version of calloc

  Allocates memory aligned to standard cache line size.
  Fill allocated memory with 0.
  Terminates application in case of any errors.
  @param[in] nmemb size_t Number of elements in array
  @param[in] size size_t Size of array elements
  @return Pointer to the allocated memory
*/
void* s_calloc(size_t nmemb, size_t size);

/**
  @brief Safe version of realloc

  Reallocates memory.
  Terminates application in case of any errors.
  @param[in] ptr void * Pointer to memory to reallocate
  @param[in] size size_t New size
  @return Pointer to the reallocated memory
*/
void* s_realloc(void *ptr, size_t size);

/**
  @brief Terminates application and print message with exit code

  @param[in] code int Exit code
*/
void s_exit(int code);

/**
  @brief Safe version of fopen

  Reallocates memory.
  Terminates application in case of any errors.
  @param[in] filename char * Path to the file to open
  @param[in] mode char * File opening mode
  @return Pointer to the file structure
*/
FILE* s_fopen(const char *filename, const char *mode);

/**
  @brief Gets size of the opened file

  @param[in] file FILE * File structure pointer
  @return Size of the file given
*/
uint64_t getFileSize(FILE * const file);

#endif /* end of include guard: STDSAFE_H */
