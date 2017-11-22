//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_TABLE_BLOCK_H
#define BIO_INF_TABLE_BLOCK_H
#define DATA(vectorTable, n_col, row, column) vectorTable.at((column) + (n_col)*(row))

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class TableBlock {
public:
    bool isCalculated = false;

    unsigned char T, COL; //dimension of block (txt). Not more than 255
    unsigned long A;
    vector<long> B, C, table;
    vector<char> X, Y;

    vector<char> lastColumn;
    vector<char> lastRow;


    // temporary patch, remove after merge
    string sX, sY, sB, sC;

    TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
            vector<char> &x, vector<char> &y, unsigned long a = 0, unsigned char numCol = 0); // offset encoded version

    void calculate();
    void emptyTemp();

    vector<char> verticalF(bool print = true); // including lowermost element
    vector<char> horizontalF(bool print = true); // including rightmost element

    void print();

    bool operator==(const TableBlock &other) const {

        return (sX == other.sX
                && sY == other.sY
                && sB == other.sB
                && sC == other.sC);
    }
};

#endif //BIO_INF_TABLE_BLOCK_H
