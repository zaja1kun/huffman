/**
  @file logging.c
  @brief Message printing functions

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#include "logging.h"
#include <stdio.h>

void printError(char const *errorText) {
    printf("%s %s%c\n", ERROR_PREFIX, errorText, '.');
}

void printInfo(char const *infoText) {
    printf("%s %s%c\n", INFO_PREFIX, infoText, '.');
}

void printUsage() {
    printf("%s\n", USAGE_MSG);
}
