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

using namespace std;

class TableBlock {
public:
    bool isCalculated = false;

    unsigned char T; //dimension of block (txt). Not more than 255
    unsigned long A;
    vector<long> B, C, table;
    string X, Y;

    vector<char> lastColumn;
    vector<char> lastRow;

    // temporary patch, remove after merge
    string sB, sC;
    string sXY;

    TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
            string x, string y, unsigned long a = 0); // offset encoded version

    void calculate();
    void emptyTemp();

    vector<char> verticalF(bool print = true); // including lowermost element
    vector<char> horizontalF(bool print = true); // including rightmost element

    void print();

    bool operator==(const TableBlock &other) const {

        return (
                  sXY == other.sXY
//                  X == other.X
//              &&  Y == other.Y
                && sB == other.sB
                && sC == other.sC);
    }
};

#endif //BIO_INF_TABLE_BLOCK_H
