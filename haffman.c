/**
  @file haffman.c
  @brief Haffman encoding and decoding algorithms

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#include "haffman.h"
#include <limits.h>
#include "btree.h"
#include "core.h"
#include "pqueue.h"


#define INBUF_T uint8_t
#define INBUF_T_SIZE (sizeof(INBUF_T)*CHAR_BIT)
#define INBUF_T_LIM (1 << INBUF_T_SIZE)
#define INBUF_T_MAX (INBUF_T_LIM - 1)
#define OUTBUF_T __uint64_t
#define OUTBUF_T_SIZE (sizeof(OUTBUF_T)*CHAR_BIT)
#define OUTBUF_T_LIM (1 << OUTBUF_T_SIZE)
#define OUTBUF_T_MAX (OUTBUF_T_LIM - 1)

/**
  Haffman table element.
*/
typedef struct {
    uint64_t code;  /**< Haffman code of the symbol */
    uint8_t len;    /**< Code length */
} htdata_t;

/**
  @brief Recursively generates haffman table using haffman tree root

  @param[in] subtreeRoot btnode_t * Pointer to the haffman tree root
  @param[in] haffmanTable htdata_t * Pointer to the haffman table
  @param[in] code uint64_t Haffman code of the previos node of the tree
  @param[in] codeLen uint8_t Level of the previos node of the tree
  @return Pointer to the generated haffman table
*/
htdata_t* btnodetoht(const btnode_t * const subtreeRoot, htdata_t *haffmanTable, uint64_t code, uint8_t codeLen) {
    if (subtreeRoot->left) {
        btnodetoht(subtreeRoot->left, haffmanTable, (code<<1)+0, codeLen+1u);
    }
    if (subtreeRoot->right) {
        btnodetoht(subtreeRoot->right, haffmanTable, (code<<1)+1, codeLen+1u);
    }
    if (!subtreeRoot->left && !subtreeRoot->right){
        haffmanTable[(uint8_t)subtreeRoot->data.symb].code = code;
        haffmanTable[(uint8_t)subtreeRoot->data.symb].len = codeLen;
    }
    return haffmanTable;
}

/**
  @brief Generates haffman table using haffman tree

  Allocates memory for the table.
  Generates table.
  Destroys haffman tree automatically
  @param[in] tree bt_t * Pointer to the haffman tree
  @see btnodetoht
  @return Pointer to the generated haffman table
*/
htdata_t* bttoht(bt_t** tree) {
    htdata_t *haffmanTable = (htdata_t*)s_calloc(256, sizeof(htdata_t));
    htdata_t *ht = btnodetoht((*tree)->root, haffmanTable, 0, 0);
    bt_free(tree);
    return ht;
}

/**
  @brief Generates Haffman tree using priority queue with symbol frequencies

  Destroys priority queue automatically
  @param[in] pq pq_t * Pointer to the priority queue
  @return Pointer to the generated haffman tree
*/
bt_t* pqtobt(pq_t **pq) {
    while (true) {
        bt_t *subtree1 = (bt_t*)pq_pop(*pq);
        bt_t *subtree2 = (bt_t*)pq_pop(*pq);
        if (!subtree1) {
            pq_free(pq);
            return NULL;
        } else if (!subtree2) {
            return subtree1;
        } else {
            bt_t *joinedTree = bt_join(&subtree1, &subtree2);
            pq_push(*pq, joinedTree, joinedTree->root->data.freq);
        }
    }
}

/**
  @brief Generates symbol frequency table

  @param[in] inBuf INBUF_T * Pointer to the text buffer
  @param[in] inBuf_size FILESIZE_T Size of the text buffer
  @return Pointer to the generated symbol frequency table
*/
FILESIZE_T* getFreqTable(INBUF_T *inBuf, FILESIZE_T inBuf_size) {
    FILESIZE_T *freqTable = (FILESIZE_T*)s_calloc(INBUF_T_LIM, sizeof(FILESIZE_T));
    for (FILESIZE_T i = 0; i < inBuf_size; i++) {
        freqTable[inBuf[i]]++;
    }
    return freqTable;
}

/**
  @brief Generates haffman code table using symbol frequency table

  Also calculates size of the encoded text and writes it to the outBuf_size.
  @param[in] freqTable FILESIZE_T * Pointer to the symbol frequency table
  @param[out] outBuf_size FILESIZE_T * Pointer to the size of the encoded text
  @return Pointer to the generated haffman code table
*/
htdata_t* getCodeTable(FILESIZE_T *freqTable, FILESIZE_T *outBuf_size) {
    // generate haffman tree using priority queue and symbol frequency table
    pq_t *pq = pq_init(INBUF_T_LIM);
    for (size_t i = 0; i < INBUF_T_LIM; i++) {
        if (freqTable[i]) {
            bt_t *symbTree = bt_init();
            btdata_t symbData = {(char)i, freqTable[i]};
            bt_insert(symbTree, symbData);
            pq_push(pq, (void*)symbTree, freqTable[i]);
        }
    }
    bt_t *freqTree = pqtobt(&pq);

    // generate code table from haffman tree
    htdata_t *haffmanTable = bttoht(&freqTree);

    // calculate output file size
    *outBuf_size = 0;
    for (uint16_t i = 0; i < 256; i++) {
        *outBuf_size += (freqTable[i]) * (haffmanTable[i].len);
    }
    *outBuf_size /= OUTBUF_T_SIZE;
    (*outBuf_size)++;

    return haffmanTable;
}

/**
  @brief Writes one symbol code to the buffer

  @param[out] buf OUTBUF_T * Buffer for encoded text
  @param[out] bufIndex FILESIZE_T * Number of the current element in the buffer
  @param[out] bufSpace int16_t * Pointer to the number of free bits in the current element of the buffer
  @param[in] symb htdata_t * Pointer to the symbol haffman code
*/
static inline void writeCodeToBuf(OUTBUF_T *buf, FILESIZE_T *bufIndex, int16_t *bufSpace, htdata_t *symb) {
    *bufSpace -= symb->len;
    if(*bufSpace >= 0) {
        buf[*bufIndex] = (buf[*bufIndex] << symb->len) | symb->code;
        if (!bufSpace) {
            bufSpace += OUTBUF_T_SIZE;
            (*bufIndex)++;
        }
    } else {
        buf[*bufIndex] = (buf[*bufIndex] << (symb->len + *bufSpace)) | (symb->code >> -*bufSpace);
        buf[++(*bufIndex)] |= (symb->code & ((1 << -*bufSpace) - 1));
        *bufSpace += OUTBUF_T_SIZE;
    }
}

void encodeFile(FILE * const input, FILE * const output) {
    printInfo(ENCODING_START);

    // copy input file data to RAM
    FILESIZE_T inBuf_size = getFileSize(input);
    INBUF_T *inBuf = (INBUF_T*)s_malloc(inBuf_size * sizeof(INBUF_T));
    fread(inBuf, inBuf_size, sizeof(INBUF_T), input);

    FILESIZE_T *freqTable = getFreqTable(inBuf, inBuf_size);
    FILESIZE_T outBuf_size = 0;
    htdata_t *codeTable = getCodeTable(freqTable, &outBuf_size);

    fwrite(freqTable, sizeof(FILESIZE_T), INBUF_T_LIM, output);
    free(freqTable);

    OUTBUF_T *outBuf = (OUTBUF_T*)s_malloc(outBuf_size*sizeof(OUTBUF_T));
    FILESIZE_T outBuf_index = 0;
    int16_t bufSpace = OUTBUF_T_SIZE;

    // encoding
    for (FILESIZE_T inBuf_index = 0; inBuf_index < inBuf_size; inBuf_index++) {
        writeCodeToBuf(outBuf, &outBuf_index, &bufSpace, codeTable + inBuf[inBuf_index]);
    }
    outBuf[outBuf_index] <<= bufSpace;

    fwrite(&bufSpace, sizeof(bufSpace), 1, output);
    fwrite(outBuf, sizeof(OUTBUF_T), outBuf_index+1, output);

    free(codeTable);
    free(inBuf);
    free(outBuf);
}

void decodeFile(FILE * const input, FILE * const output) {
    printInfo(DECODING_START);

    // read freqTable from file
    FILESIZE_T *freqTable = (FILESIZE_T*)s_malloc(INBUF_T_LIM*sizeof(FILESIZE_T));
    fread(freqTable, INBUF_T_LIM, sizeof(FILESIZE_T), input);

    // calculate size of original text
    FILESIZE_T outBuf_size = 0;
    for (size_t i = 0; i < INBUF_T_LIM; i++) {
        outBuf_size += freqTable[i];
    }
    INBUF_T *outBuf = (INBUF_T*)s_malloc(outBuf_size * sizeof(INBUF_T));

    // read number of free bytes in the end of the file
    int16_t bufSpace = 0;
    fread(&bufSpace, 1, sizeof(bufSpace), input);

    // read encoded text from the file
    FILESIZE_T inBuf_size = (getFileSize(input) - sizeof(FILESIZE_T)*INBUF_T_LIM - sizeof(bufSpace)) / sizeof(OUTBUF_T);
    OUTBUF_T *inBuf = (OUTBUF_T*)s_malloc(inBuf_size * sizeof(OUTBUF_T));
    fread(inBuf, inBuf_size, sizeof(OUTBUF_T), input);

    // generate haffman tree using priority queue and symbol frequency table
    pq_t *pq = pq_init(INBUF_T_LIM);
    for (size_t i = 0; i < INBUF_T_LIM; i++) {
        if (freqTable[i]) {
            bt_t *symbTree = bt_init();
            btdata_t symbData = {(char)i, freqTable[i]};
            bt_insert(symbTree, symbData);
            pq_push(pq, (void*)symbTree, freqTable[i]);
        }
    }
    bt_t *freqTree = pqtobt(&pq);

    FILESIZE_T data_size = inBuf_size * OUTBUF_T_SIZE - bufSpace;
    FILESIZE_T outBuf_index = 0;
    FILESIZE_T inBuf_index = 0;
    int8_t curBuf_curBit = OUTBUF_T_SIZE - 1;
    btnode_t *curNode = freqTree->root;

    for (FILESIZE_T data_index = 0; data_index < data_size; data_index++) {
        if ((inBuf[inBuf_index] >> curBuf_curBit) & 1U) {
            curNode = curNode->right;
        } else {
            curNode = curNode->left;
        }
        if (!((bool)curNode->left | (bool)curNode->right)) {
            outBuf[outBuf_index++] = curNode->data.symb;
            curNode = freqTree->root;
        }
        if (!curBuf_curBit) {
            curBuf_curBit = OUTBUF_T_SIZE - 1;
            inBuf_index++;
        } else {
            curBuf_curBit--;
        }
    }

    fwrite(outBuf, sizeof(INBUF_T), outBuf_size, output);

    bt_free(&freqTree);
    free(inBuf);
    free(outBuf);
    free(freqTable);
}
