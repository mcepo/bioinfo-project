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

class TableBlock {
public:
    bool isCalculated = false;

    unsigned char T; //dimension of block (txt). Not more than 255

    vector<char> B, C, XY, table;
    string X, Y;

    vector<char> lastColumn;
    vector<char> lastRow;

    TableBlock( vector<char> &b, vector<char> &c,
            string x, string y); // offset encoded version

    void calculate();
    void emptyTemp();

    vector<char> verticalF(bool print = true); // including lowermost element
    vector<char> horizontalF(bool print = true); // including rightmost element

    void print();

    bool operator==(const TableBlock &other) const {

        return (
                 XY == other.XY
                && B == other.B
                && C == other.C);
    }
};

#endif //BIO_INF_TABLE_BLOCK_H
