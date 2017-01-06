/**
  @file huffman.h
  @brief Huffman encoding and decoding algorithms

  @author Zaitsev Yury
  @copyright Copyright (c) 2016, Zaitsev Yury
  @license This file is released under the GNU Public License
*/
#ifndef HAFFMAN_H
#define HAFFMAN_H

#include <stdio.h>

/**
  @brief Huffman code encoder

  @param[in] input FILE * File to encode
  @param[in] output FILE * File to write code to
*/
void encodeFile(FILE * const input, FILE * const output);

/**
  @brief Huffman code decoder

  @param[in] input FILE * File to decode
  @param[in] output FILE * File to write decoded text to
*/
void decodeFile(FILE * const input, FILE * const output);

#endif /* end of include guard: HAFFMAN_H */
