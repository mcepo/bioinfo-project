//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_FOUR_RUSSIANS_H
#define BIO_INF_FOUR_RUSSIANS_H
#define DATA(vectorTable, n_col, row, column) vectorTable[(column) + (n_col)*(row)]

#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#define BLOCK_SIZE 3

using namespace std;

// TODO: preseliti u posebnu datoteku

struct Block {
    char *B, *C, *XY;
    int xLength, yLength;
    char *lastRow, *lastColumn;

    bool operator==(const Block &other) const {

        if (other.xLength != xLength || other.yLength != yLength) {
            return false;
        }

        int sum = xLength + yLength;

        // not comparing first character because its always the same -> 0 :)
        for (int i = 1; i < sum; i++) {
            if (XY[i] != other.XY[i]) {
                return false;
            }
        }
        for (int i = 0; i < xLength; i++) {
            if (B[i] != other.B[i]) {
                return false;
            }
        }
        for (int i = 0; i < yLength; i++) {
            if (C[i] != other.C[i]) {
                return false;
            }
        }
        return true;
    }
};

// TODO: preseliti u posebnu datoteku
// TODO: koliko je kvalitetno ovo rješenje ?
namespace std {

    template <>
    struct hash<Block> {

        std::size_t operator()(const Block& k) const {

            size_t seed = 0;
            int sum = k.xLength + k.yLength;
            size_t res = 17;

            for (int i = 0; i < sum; i++) {
                seed ^= std::hash<char>()(k.XY[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            res = res * 31 + seed;
            seed = 0;
            for (int i = 0; i < k.xLength; i++) {
                seed ^= std::hash<char>()(k.B[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            res = res * 31 + seed;
            seed = 0;
            for (int i = 0; i < k.yLength; i++) {
                seed ^= std::hash<char>()(k.C[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            res = res * 31 + seed;
            return res;
        }
    };
}

class FourRussians {
public:

    unsigned char T; //dimension of block (txt). Not more than 255

    char *table;
    char *constBC;

    long top, left, diagonal;

    unsigned char modX, modY;

    int xLen, yLen, numBlocksPerRow, numRowsToCalculate;
    string X, Y;

    unordered_map<Block, Block> generatedBlocks;

    int found;
    Block* blocks;

    FourRussians(string x, string y, int blockSize); // offset encoded version
    FourRussians();

    unsigned long calculate();
    Block& getTableBlock(char *b, char *c,
            string const& x, string const& y,
            unsigned char xLength, unsigned char yLength);

    void calculateBlock(Block &blk);
    void calculateRow(int index, unsigned char yLength);

    char* getXY(string const& x, string const& y,
            unsigned char xLength, unsigned char yLength);

    void print(Block &blk, unsigned char xLength, unsigned char yLength);
};

#endif //BIO_INF_FOUR_RUSSIANS_H
