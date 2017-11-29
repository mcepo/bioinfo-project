//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_FOUR_RUSSIANS_H
#define BIO_INF_FOUR_RUSSIANS_H
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

namespace std {
    template<typename T>
    struct hash<vector<T>>
    {

        std::size_t operator()(const vector<T>& in) const {
            size_t size = in.size();
            size_t seed = 0;
            for (size_t i = 0; i < size; i++)
                seed ^= std::hash<T>()(in[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };

    template <>
    struct hash<Block> {

        std::size_t operator()(const Block& k) const {

            size_t res = 17;

            res = res * 31 + hash<vector<char>>()(k.XY);
            res = res * 31 + hash<vector<char>>()(k.B);
            res = res * 31 + hash<vector<char>>()(k.C);
            return res;
        }
    };
}

class FourRussians {
public:

    unsigned char T; //dimension of block (txt). Not more than 255

    vector<char> table;
    int xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;


    unordered_map<Block, Block> generatedBlocks;

    int found;

    vector<Block> blocks;

    FourRussians(string x, string y, int blockSize); // offset encoded version
    FourRussians();

    unsigned long calculate();
    Block getTableBlock(vector<char> &b, vector<char> &c,
            string const& x, string const& y);
    void calculateBlock(Block &blk);
    vector<char> getXY(string x, string y);

    void print();
};

#endif //BIO_INF_FOUR_RUSSIANS_H
