//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_FOUR_RUSSIANS_H
#define BIO_INF_FOUR_RUSSIANS_H
#define DATA(vectorTable, n_col, row, column) vectorTable[(column) + (n_col)*(row)]

#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#define BLOCK_SIZE 3

using namespace std;

// TODO: preseliti u posebnu datoteku

struct Block {
    char xLength;
    char *lastRow, *lastColumn;
};

class FourRussians {
public:

    unsigned char T; //dimension of block (txt). Not more than 255

    char *table;
    char *constBC;

    long top, left, diagonal;

    unsigned char modX, modY;

    int xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;
    
    Block** generatedBlocksArray;

    int found;
    Block** blocks;

    FourRussians(string x, string y, int blockSize); // offset encoded version
    FourRussians();

    unsigned long calculate();
    Block* getTableBlock(char *b, char *c,
            string const& x, string const& y,
            unsigned char xLength, unsigned char yLength);

    Block* calculateBlock(char *b, char *c,
            string const& x, string const& y,
            unsigned char xLength, unsigned char yLength);
    void calculateRow(int index, unsigned char yLength);
    
    unsigned long numCombinations(unsigned char T);
    char index_to_acgt(unsigned int index);
    unsigned int acgt_to_index(const char acgt);
    unsigned int bc_to_index(char bc);
    unsigned long lookUpIndex(unsigned char xIndex, unsigned char yIndex, string const& x, string const& y, char *b, char *c);


    void print(Block &blk, unsigned char xLength, unsigned char yLength);
};

#endif //BIO_INF_FOUR_RUSSIANS_H
