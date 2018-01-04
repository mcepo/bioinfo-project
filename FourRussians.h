//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_FOUR_RUSSIANS_H
#define BIO_INF_FOUR_RUSSIANS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

#include "ThreadPool.h"

#include <bitset>

#define BLOCK_SIZE 3

using namespace std;

class FourRussians {
public:
    uint8_t T; //dimension of block (txt). Not more than 255
    uint8_t optimalT;

// structure used as storage when calculating lastRow and lastColumn
// in method calculateBlock
    int8_t **table;

// storing value for c & b of blocks in firstRow and firstColumn
    uint8_t firstRC;
    uint8_t mask;
    uint64_t numComb;
    
    uint32_t xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;

// matrix
    uint32_t **matrix;

// storage of all calculated blocks
    uint16_t* genBlocks;

// calculated hash values for x & y string
    uint8_t *xHash;
    uint8_t *yHash;
    
    FourRussians(string const &x, string const &y, int blockSize); // offset encoded version
    FourRussians();

// returns the max number of calculated blocks
    uint64_t numCombinations();
    
// generates hashes for strings x & y
    void generateXYHashes();
    
// calculates optimal value for T for a given input strings
// stores it in optimalT variable
    void optimalTcalc();
    
// start calculating matrix
    void calculateMatrix();
    
// calculating edit script
// returns edit distance
// writes edit script into a file
    uint32_t calculateEditDistanceAndScript();

// calculate a single block used when calculating edit script and distance
    void calculateBlock(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
            
// calculate a single block used when precalculating blocks
    uint16_t calculateBlockFast(uint8_t rowIndex, uint8_t colIndex, uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);
    
// generating all blocks for matrix
    void generateBlocks( uint8_t index, uint8_t rowIndex, uint8_t colIndex, uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c);
    
// just merges hashes of b,c,x,y for a given block and returns a merged hash
// that is used as a index when searching and storing blocks    

    uint32_t mergeHash(uint8_t xHash, uint8_t yHash,
            uint8_t b, uint8_t c);

    uint8_t acgt_to_index(const char acgt);

    void print(uint8_t xHash, uint8_t yHash);
};

#endif //BIO_INF_FOUR_RUSSIANS_H
