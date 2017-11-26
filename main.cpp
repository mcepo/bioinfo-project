#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

#include "TableBlock.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"
#define BLOCK_SIZE 4

namespace std {

    template <>
    struct hash<TableBlock> {

        std::size_t operator()(const TableBlock& k) const {

            size_t res = 17;

            //  res = res * 31 + hash<string>()(k.sX);
            //  res = res * 31 + hash<string>()(k.sY);
            res = res * 31 + hash<string>()(k.sXY);
            res = res * 31 + hash<string>()(k.sB);
            res = res * 31 + hash<string>()(k.sC);
            return res;
        }
    };
}

unordered_map<TableBlock, TableBlock> generatedBlocks;

int calculated = 0;
int found = 0;

TableBlock getTableBlock(unsigned char t, vector<char> &b, vector<char> &c,
        string x, string y, unsigned long a = 0) {

    TableBlock blk(t, b, c, x, y, a);

    auto foundBlock = generatedBlocks.find(blk);

    if (foundBlock == generatedBlocks.end()) {
        calculated++;
        blk.calculate();
        generatedBlocks.insert({blk, blk});
    } else {
        blk = foundBlock->second;
        found++;
    }
    return blk;
}

int main(int argc, char** argv) {

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

    int xLen = X.size();
    int yLen = Y.size();

    int numBlocksPerRow = xLen / blockSize;
    int numRowsToCalculate = yLen / blockSize;

    vector<TableBlock> blocks;
    blocks.reserve((unsigned long) numBlocksPerRow);

    // the magic

    //int row = 0;
    vector<char> currentC, currentB;
    currentB.reserve((unsigned long) blockSize);
    currentC.reserve((unsigned long) blockSize);

    for (int row = 0; row < numRowsToCalculate; row++) {

        //    cout << "------------------------" << endl;

        if (row == 0) {
            for (int i = 0; i < blockSize; i++) {
                currentB.push_back(+1); // ok
            }
        }

        for (int col = 0; col < numBlocksPerRow; col++) {

            if (row != 0) {
                currentB.clear();
                currentB = blocks.at((unsigned int) col).horizontalF(false);
            }

            if (col == 0) {
                currentC.clear();
                for (int i = 0; i < blockSize; i++) { // do dynamicaly
                    currentC.push_back(+1); // ok
                }
            }

            TableBlock blk = getTableBlock(blockSize, currentB, currentC, &X[BLOCK_SIZE * col], &Y[BLOCK_SIZE * row], 0);
            if (row == 0) {
                blocks.push_back(blk);
            } else {
                blocks.at((unsigned int) col) = blk;
            }

            //            cout << "################" << endl;

            //            cout << "r: " << row << " c: " << col << endl;
            //            blocks.at((unsigned long) col).print();
            //            cout << "V: ";
            currentC = blocks.at((unsigned long) col).verticalF(false);
            //            cout << "H: ";
            blocks.at((unsigned long) col).horizontalF(false);
        }
    }

    unsigned long result = 0L;

    for (auto &block : blocks) {
        for (unsigned long j = 0; j < block.horizontalF(false).size(); j++) {
            result += block.horizontalF(false).at(j);
        }
    }

    result += yLen;

    const char * X_char = X.c_str();
    const char * Y_char = Y.c_str();

    cout << "Input string length: " << xLen << endl;
    cout << "Total number of blocks in matrix: " << (numBlocksPerRow * numRowsToCalculate) << endl;
    cout << "Calculated (stored) blocks: " << calculated << endl;
    cout << "Found blocks: " << found << endl;
    cout << "RESULT: " << result;

    // EDLIB controle

    if (xLen != 0 && yLen != 0) {
        EdlibAlignResult resultCheck = edlibAlign(X_char, xLen, Y_char, yLen, edlibDefaultAlignConfig());
        if (resultCheck.status == EDLIB_STATUS_OK) {
            //      printf("\n*********** \n Edlib control check -> edit_distance = %d\n", resultCheck.editDistance);
        }
        edlibFreeAlignResult(resultCheck);

        if ((result - resultCheck.editDistance) == 0) {
            cout << " ***** PASSED";
        } else {
            cout << " ********** FAILED  " << endl;
            cout << " Result diff: " << (result - resultCheck.editDistance) << " (0 for valid execution)";
        }
    }
    cout << endl;
    return 0;
}
