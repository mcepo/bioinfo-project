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
    uint8_t T; //dimension of block (txt). Not more than 255

    // structure used as storage when calculating lastRow and lastColumn
    // in method calculateBlock
    int8_t **table;

    // storing value for c & b of blocks in firstRow and firstColumn
    uint8_t firstRC;

    // used in method calculateBlock as temp variables
    long top, left, diagonal;

    int xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;

// storage of last row when calculating
    uint16_t* blocks;
// storage of all calculated blocks
    uint16_t* genBlocks;

// calculated hash values for x & y string
    uint8_t *xHash;
    uint8_t *yHash;

    FourRussians(string const &x, string const &y, int blockSize); // offset encoded version
    FourRussians();

// returns the max number of calculated blocks
    unsigned long numCombinations();
// generates hashes for strings x & y
    void generateXYHashes();
// start calculating edit distance
    unsigned long calculate();
    
// calculate a single row of matrix
    void calculateRow(int index);

// calcualte a single block
    uint16_t calculateBlock(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
    
// method that checks if a block is already calculated and stored
// or it should be calculated, if so calls calculateBlock to calculate the new
// block and returns the calculated block
    uint16_t getTableBlock(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
    
// just merges hashes of b,c,x,y for a given block and returns a merged hash
// that is used as a index when searching and storing blocks    

    uint32_t mergeHash(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);

    uint8_t acgt_to_index(const char acgt);

    void print(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c);
};

#endif //BIO_INF_FOUR_RUSSIANS_H
