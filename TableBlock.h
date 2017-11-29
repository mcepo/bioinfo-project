//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_TABLE_BLOCK_H
#define BIO_INF_TABLE_BLOCK_H
#define DATA(vectorTable, n_col, row, column) vectorTable.at((column) + (n_col)*(row))

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#define BLOCK_SIZE 3

using namespace std;

struct Block {
    vector<char> B, C, XY, lastRow, lastColumn;

    bool operator==(const Block &other) const {

        return (
                XY == other.XY
                && B == other.B
                && C == other.C);
    }

};

class TableBlock {
public:

    unsigned char T; //dimension of block (txt). Not more than 255

    vector<char> table;
    string X, Y;

    TableBlock( string x, string y, int blockSize); // offset encoded version
    TableBlock(); 

    void calculateBlock(Block &blk);
    
    vector<char> getXY(string x, string y);
    
    void print();
};

#endif //BIO_INF_TABLE_BLOCK_H
