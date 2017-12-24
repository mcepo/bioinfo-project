//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_FOUR_RUSSIANS_H
#define BIO_INF_FOUR_RUSSIANS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "ThreadPool.h"

#define BLOCK_SIZE 4

using namespace std;

class FourRussians {
public:
    // Tp === T + 1
    unsigned char T, Tp; //dimension of block (txt). Not more than 255

    // pomočna struktura koja se koristi u metodi calculateBlock
    // za izračun bloka
    int8_t **table;

    uint8_t firstRC;

    long top, left, diagonal;

    int xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;

    int found;
    int calculated; 
    uint16_t* blocks;
    uint16_t* genBlocks;

    uint8_t **xHash;
    uint8_t *yHash;

    FourRussians(string x, string y, int blockSize); // offset encoded version
    FourRussians();

    unsigned long numCombinations(unsigned char T);
    void generateXYHashes();
    unsigned long calculate();
    void calculateRow(int index);
    uint16_t calculateBlock(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
    uint16_t getTableBlock(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
    uint32_t mergeHash(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);

    //  unsigned int acgt_to_index(const char acgt);

    void print(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c);
};

#endif //BIO_INF_FOUR_RUSSIANS_H
