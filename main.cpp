#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <time.h>

#include "TableBlock.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"

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

unordered_map<Block, Block> generatedBlocks;

int found = 0;

TableBlock tblBlk;

Block getTableBlock(vector<char> &b, vector<char> &c,
        string const& x, string const& y) {

    Block blk = {b, c, tblBlk.getXY(x, y)};

    auto foundBlock = generatedBlocks.find(blk);

    if (foundBlock == generatedBlocks.end()) {
        tblBlk.calculateBlock(blk);
        generatedBlocks.insert({blk, blk});
    } else {
        blk = foundBlock->second;
        found++;
    }
    return blk;
}

int main(int argc, char** argv) {

    clock_t start = clock();
    string inputFilename, X, Y;

    int blockSize = BLOCK_SIZE;
    
    if (argc != 2) {
        inputFilename = DEFAULT_FILENAME;
    } else {
        inputFilename = argv[1];
    }

    ifstream inputFile(inputFilename);

    if (inputFile) {
        getline(inputFile, X);
        getline(inputFile, Y);
    } else {
        cout << "Unable to open file";
        return -1;
    }

    int modX = X.size() % blockSize;
    if (modX != 0) {
        int appendLengthX = blockSize - modX;
        for (int i = 0; i < appendLengthX; i++) {
            X += '*';
        }
    }
    int modY = Y.size() % blockSize;
    if (modY != 0) {
        int appendLengthY = blockSize - modY;
        for (int i = 0; i < appendLengthY; i++) {
            Y += '*';
        }
    }
    
    tblBlk = TableBlock(X, Y, blockSize);

    int xLen = X.size();
    int yLen = Y.size();

    int numBlocksPerRow = xLen / blockSize;
    int numRowsToCalculate = yLen / blockSize;

    vector<Block> blocks;
    blocks.reserve((unsigned long) numBlocksPerRow);

    // the magic

    // first row calculation
    vector<char> currentC, currentB;
    currentB.reserve((unsigned long) blockSize);
    currentC.reserve((unsigned long) blockSize);

    for (int i = 0; i < blockSize; i++) {
        currentB.push_back(+1); // ok
        currentC.push_back(+1); // ok
    }

    for (int col = 0; col < numBlocksPerRow; col++) {

        blocks.push_back(getTableBlock(currentB, currentC, &X[blockSize * col], &Y[0]));
        currentC = blocks.at((unsigned long) col).lastColumn;
    }

    // rest of the matrix

    for (int row = 1; row < numRowsToCalculate; row++) {

        currentC.clear();
        for (int i = 0; i < blockSize; i++) { // do dynamicaly
            currentC.push_back(+1); // ok
        }

        for (int col = 0; col < numBlocksPerRow; col++) {

            currentB = blocks.at((unsigned int) col).lastRow;
            blocks.at((unsigned int) col) = getTableBlock(currentB, currentC, &X[blockSize * col], &Y[blockSize * row]);
            currentC = blocks.at((unsigned long) col).lastColumn;
        }
    }

    unsigned long result = 0L;

    for (auto &block : blocks) {
        for (unsigned long j = 0; j < block.lastRow.size(); j++) {
            result += block.lastRow.at(j);
        }
    }

    result += yLen;

    const char * X_char = X.c_str();
    const char * Y_char = Y.c_str();

    double execTime = (clock() - start) / (double) CLOCKS_PER_SEC;

    cout << "Input string length: " << xLen << endl;
    cout << "Total number of blocks in matrix: " << (numBlocksPerRow * numRowsToCalculate) << endl;
    cout << "Calculated (stored) blocks: " << generatedBlocks.size() << endl;
    cout << "Found blocks: " << found << endl;
    cout << "RESULT: " << result << " generated in: " << execTime << "sec" << endl;

    // EDLIB controle

    if (xLen != 0 && yLen != 0) {
        start = clock();
        EdlibAlignResult resultCheck = edlibAlign(X_char, xLen, Y_char, yLen, edlibDefaultAlignConfig());
        if (resultCheck.status == EDLIB_STATUS_OK) {
            //      printf("\n*********** \n Edlib control check -> edit_distance = %d\n", resultCheck.editDistance);
        }
        edlibFreeAlignResult(resultCheck);
        execTime = (clock() - start) / (double) CLOCKS_PER_SEC;
        if ((result - resultCheck.editDistance) == 0) {
            cout << " *** PASSED ***  check in " << execTime << "sec" << endl;
        } else {
            cout << " *** FAILED  *** check in " << execTime << "sec" << endl;
            cout << " Result diff: " << (long) (result - resultCheck.editDistance) << " (0 for valid execution)" << endl;
        }
    }
    return 0;
}
