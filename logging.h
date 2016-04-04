/**
  @file logging.h
  @brief Message printing functions

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#ifndef LOGGING_H
#define LOGGING_H

#include "msg.h"

/**
  @brief Prints error message with special prefix

  @param[in] errorText char* Message
*/
void printError(char const *errorText);

/**
  @brief Prints informational message with special prefix

  @param[in] infoText char* Message
*/
void printInfo(char const *infoText);

/**
  @brief Prints application usage help

*/
void printUsage();

#endif /* end of include guard: LOGGING_H */
