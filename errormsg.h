#ifndef ERRORMSG_H
#define ERRORMSG_H

// core.c
#define WRONG_ARG_NUM "wrong number of arguments given"
#define WRONG_ARG "wrong argument given"

// stdsafe.c
#define S_MALLOC_FAILED "memory can't be allocated"
#define S_REALLOC_FAILED "memory can't be reallocated"
#define S_FOPEN_FAILED "can\'t open file"
#define S_EXIT_MSG "app closed unexpectedly with exit code"

// logging.c
#define USAGE_MSG "Usage:\n  huff ifile [-c|-x] ofile"
#define ERROR_PREFIX "Error:"
#define INFO_PREFIX "Info:"

// haffman.c
#define ENCODING_START "file encoding started"
#define DECODING_START "file decoding started"

#endif /* end of include guard: ERRORMSG_H */
