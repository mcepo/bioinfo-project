//
// Created by Tomislav Lokotar on 11/4/17.
//
#ifndef BIO_INF_TABLE_BLOCK_H
#define BIO_INF_TABLE_BLOCK_H
#define DATA(vectorTable, n_col, row, column) vectorTable.at((column) + (n_col)*(row))
//#define DATA2(vectorTable, n_col, row, column) vectorTable[(column) + (n_col)*(row)]

//#define DUMP_ROW "/home/loki/dumpRow.bin"
//#define DUMP_COL "/home/loki/dumpCol.bin"

#define DUMP_PATH "/tmp/bioinfo/dump.bin"

#define MAX_SPACE 509607936
#define T4_CACHE_SIZE 429981696

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <fstream>
#include <vector>

using namespace std;



class blockInput{
    public:
        blockInput(unsigned char T, vector<char>* b, vector<char>* c, const char *x, const char *y);
        unsigned char T;
        vector<char>* b;
        vector<char>* c;
        const char *x;
        const char *y;
};

class blockOutput{
    public:
        blockOutput() = default;
        vector<char>* H;
        vector<char>* V;
        unsigned long sum();
        //unsigned long sum(unsigned char limit);
        //void print();
};

unsigned long combineBC(char bIndex, char cIndex);
unsigned long combineXY(unsigned long xIndex, unsigned long yIndex);
unsigned long lookUpIndex4(unsigned long xy, unsigned long bc);
uint8_t* loadCacheToArray4(const char *inPath);
void unpack4push(uint8_t dense, vector<char>* out, unsigned char T);
void unpack4(uint8_t dense, vector<char>* out, unsigned char T);
char index_to_acgt(unsigned int index);
//void cartesian( vector<vector<char> >& v );
uint8_t compact(vector<char>* element, unsigned char T = 3U);
void unpack(uint8_t dense, vector<char>* out, unsigned char T, bool push=true);
//unsigned int acgt_to_index(char acgt);
unsigned int bc_to_index(char bc);
unsigned long lookUpIndex(unsigned char T, const char *x, const char *y, vector<char>* b, vector<char>* c);
void genIndex();
void reserveMemory();
void generateAll();
unsigned long acgtIndex4(const char *acgt);
vector<uint8_t >* loadCache(const char *inPath, unsigned char T);

void combinationGenerator(unsigned char T, const char *outPathH, const char *outPathV);

class TableBlock{
public:
    bool isCalculated = false;

    unsigned char T;            //dimension of block (txt). Not more than 255
    //unsigned long A;
    vector<int> B, C, table;
    unsigned int * table2;
    string X, Y;
    string *Xstr;
    string *Ystr;
    const char* Xs;
    const char* Ys;
    blockOutput* resultCache;

    vector<char> lastColumn;
    vector<char> lastRow;

    //TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
     //          vector<char> &x, vector<char> &y, unsigned long a = 0, unsigned char numCol = 0); // offset encoded version

    TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
               const char *x, const char *y);

    TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
               string *x, string *y);

    TableBlock(unsigned char t, vector<char>* b, vector<char>* c,
                           const char *x, const char *y);

    ~TableBlock();

    explicit TableBlock(blockInput* in);

    blockOutput* result();

    void calculate();
    //void calculate2();
    void emptyTemp();

    vector<char>* verticalF(bool print = false);     // including lowermost element
    vector<char>* horizontalF(bool print = false);   // including rightmost element

    void print();
    //void print2();
    bool operator==(const TableBlock &other) const;



};

namespace std
{
    template <>
    struct hash<TableBlock>
    {
        size_t operator()(const TableBlock& blk) const
        {

            size_t seed = blk.T;
            for(unsigned long i = 0; i < blk.T; i++){
                seed ^= blk.B.at(i) + 0x421377b9 + (seed << 6) + (seed >> 2);
                seed ^= blk.C.at(i) + 0x1de6dcfc + (seed << 6) + (seed >> 2);
                seed ^= blk.Xs[i]   + 0xaf6683d4 + (seed << 6) + (seed >> 2);
                seed ^= blk.Ys[i]   + 0x6a41376a + (seed << 6) + (seed >> 2);
            }
            //cout << seed;
            //cout << endl;
            return seed;
        }
    };
}

#endif //BIO_INF_TABLE_BLOCK_H