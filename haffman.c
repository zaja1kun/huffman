#include "haffman.h"
#include <math.h>
#include "core.h"
#include "pqueue.h"
#include "btree.h"
#include "asciitree.h"


typedef struct {
    uint64_t code;
    uint8_t len;
} htdata_t;


htdata_t* getHaffmanTable(const btnode_t * const subtreeRoot, htdata_t *haffmanTable, uint64_t code, uint8_t codeLen) {
    if (subtreeRoot->left) {
        getHaffmanTable(subtreeRoot->left, haffmanTable, (code<<1)+0, codeLen+1);
    }
    if (subtreeRoot->right) {
        getHaffmanTable(subtreeRoot->right, haffmanTable, (code<<1)+1, codeLen+1);
    }
    if (!subtreeRoot->left && !subtreeRoot->right){
        haffmanTable[(uint8_t)subtreeRoot->data.symb].code = code;
        haffmanTable[(uint8_t)subtreeRoot->data.symb].len = codeLen;
    }
    return haffmanTable;
}

htdata_t* bttoht(const bt_t* const tree) {
    htdata_t *haffmanTable = (htdata_t*)s_calloc(256, sizeof(htdata_t));
    return getHaffmanTable(tree->root, haffmanTable, 0, 0);
}

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


bool encodeFile(FILE * const input, FILE * const output) {
    printInfo(ENCODING_START);
    // copy input file data to RAM
    uint64_t inputSize = getFileSize(input);
    char *inputData = (char*)s_malloc(inputSize * sizeof(char));
    fread(inputData, inputSize, 1, input);
    // calculate symbols occurence frequency in the input data
    uint64_t symbFreq[256] = {0};
    for (size_t i = 0; i < inputSize; i++) {
        symbFreq[(uint8_t)inputData[i]]++;
    }
    // generate haffman tree using priority queue
    pq_t *pq = pq_init(256);
    for (size_t i = 0; i < 256; i++) {
        if (symbFreq[i]) {
            bt_t *symbTree = bt_init();
            btdata_t symbData = {(char)i, symbFreq[i]};
            bt_insert(symbTree, symbData);
            pq_push(pq, (void*)symbTree, symbFreq[i]);
        }
    }
    bt_t *freqTree = pqtobt(&pq);
    print_ascii_tree(freqTree->root);

    htdata_t *haffmanTable = bttoht(freqTree);
    bt_free(&freqTree);
    for (size_t i = 0; i < 256; i++) {
        if(haffmanTable[i].len) {
            printf("%c - %lx, %u\n", (char)i, haffmanTable[i].code, haffmanTable[i].len);
        }
    }

///////////////////////////////////////////////////////////////////////////////////
    uint64_t outputSize = inputSize/sizeof(uint64_t)<2?2:inputSize/sizeof(uint64_t);
    uint64_t *outputData = (uint64_t*)s_malloc(outputSize*sizeof(uint64_t));
    uint64_t inputBufIndex = 0;
    uint64_t outputBufIndex = 0;
    int8_t bufFreeSpace = sizeof(uint64_t)*8;

    while (inputBufIndex < inputSize) {
        htdata_t *symb = haffmanTable + inputData[inputBufIndex];
        bufFreeSpace -= symb->len;
        if(bufFreeSpace >= 0) {
            outputData[outputBufIndex] <<= symb->len;
            outputData[outputBufIndex] += symb->code;
            if (bufFreeSpace == 0) {
                bufFreeSpace += sizeof(uint64_t);
                outputBufIndex++;
            }
        } else {
            outputData[outputBufIndex] <<= (symb->len + bufFreeSpace);
            outputData[outputBufIndex] += (symb->code >> -bufFreeSpace);
            outputBufIndex++;
            // outputData[outputBufIndex] << (-bufFreeSpace);
            outputData[outputBufIndex] += (symb->code & ((uint64_t)pow(2, -bufFreeSpace) - 1));
            bufFreeSpace += sizeof(uint64_t)*8;
        }
        if (outputBufIndex + 1 > outputSize) {
            outputSize *= 2;
            outputData = (uint64_t*)s_realloc(outputData, outputSize);
        }
        inputBufIndex++;
    }
    outputData[outputBufIndex] <<= bufFreeSpace;
    free(haffmanTable);
    haffmanTable = NULL;
///////////////////////////////////////////////////////////////////////////////////

    free(inputData);
    inputData = NULL;
    fwrite(outputData, sizeof(uint64_t), outputBufIndex+1, output);
    free(outputData);
    outputData = NULL;
    return true;
}

bool decodeFile(FILE * const input, FILE * const output) {
    printInfo(DECODING_START);

    uint64_t inputSize = getFileSize(input);
    uint64_t outputSize = getFileSize(output);

    inputSize++;
    outputSize++;

    return true;
}
