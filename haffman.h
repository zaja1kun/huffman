#ifndef HAFFMAN_H
#define HAFFMAN_H

#include <stdio.h>
#include <stdbool.h>

bool encodeFile(FILE * const input, FILE * const output);
bool decodeFile(FILE * const input, FILE * const output);

#endif /* end of include guard: HAFFMAN_H */
