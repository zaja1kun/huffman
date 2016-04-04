/**
  @file core.c
  @brief Haffman algorithm launcher

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#include "core.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "haffman.h"

/**
  @brief Application entry point

  Analizes command line arguments, openes files.
  @param[in] argc int Number of command line arguments given
  @param[in] argv char*[] Array of command line arguments
  @return 0
*/
int main(int argc, char const *argv[]) {
    time_t start = clock();

    if(argc != 4) {
        printError(WRONG_ARG_NUM);
        printUsage();
        exit(0);
    }

    FILE *input = s_fopen(argv[1], "rb");
    FILE *output = s_fopen(argv[3], "wb");

    if (!strcmp(argv[2], "-c")) {
        encodeFile(input, output);
    } else if (!strcmp(argv[2], "-x")) {
        decodeFile(input, output);
    } else {
        printError(WRONG_ARG);
        printUsage();
    }

    fclose(input);
    fclose(output);

    time_t end = clock();
    printf("Info: spent %.4f seconds.\n", (float)(end - start) / (CLOCKS_PER_SEC));
    return 0;
}
