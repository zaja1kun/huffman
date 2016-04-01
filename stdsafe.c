#include "stdsafe.h"
#include <stdlib.h>
#include "core.h"

void* s_malloc(size_t size) {
    void *buf = malloc(size);
    if(!buf && size) {
        printError(S_MALLOC_FAILED);
        s_exit(0);
    }
    return buf;
}

void *s_calloc(size_t nmemb, size_t size) {
    void *buf = calloc(nmemb, size);
    if(!buf && size) {
        printError(S_MALLOC_FAILED);
        s_exit(0);
    }
    return buf;
}

void *s_realloc(void *ptr, unsigned size) {
    void *buf = realloc(ptr, size);
    if(!buf && size) {
        printError(S_REALLOC_FAILED);
        s_exit(0);
    }
    return buf;
}

void s_free(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

FILE* s_fopen(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if(file == NULL) {
        size_t errorMsgLength = snprintf(NULL, 0, "%s %s", S_FOPEN_FAILED, filename) + 1;
        char *errorMsg = (char*)s_malloc(errorMsgLength*sizeof(char));
        snprintf(errorMsg, errorMsgLength, "%s %s", S_FOPEN_FAILED, filename);
        printError(errorMsg);
        free(errorMsg);
        errorMsg = NULL;
        s_exit(0);
    }
    return file;
}

void s_exit(int code) {
    size_t infoMsgLength = snprintf(NULL, 0, "%s %d", S_EXIT_MSG, code) + 1;
    char *infoMsg = (char*)s_malloc(infoMsgLength*sizeof(char));
    snprintf(infoMsg, infoMsgLength, "%s %d", S_EXIT_MSG, code);
    printInfo(infoMsg);
    exit(0);
}

uint64_t getFileSize(FILE * const file) {
    uint64_t curPos = ftell(file);
    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, curPos, SEEK_SET);
    return size;
}
