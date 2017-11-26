#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TableBlock.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"
#define BLOCK_SIZE 4

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

    cout << X.size() << endl;

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

    cout << xLen << endl;

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

            TableBlock blk(blockSize, currentB, currentC, &X[BLOCK_SIZE * col], &Y[BLOCK_SIZE * row], 0);
            blk.calculate();
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

    // EDLIB CONTROL

    const char * X_char = X.c_str();
    const char * Y_char = Y.c_str();

    EdlibAlignResult resultCheck = edlibAlign(X_char, X.size(), Y_char, Y.size(), edlibDefaultAlignConfig());
    if (resultCheck.status == EDLIB_STATUS_OK) {
        //      printf("\n*********** \n Edlib control check -> edit_distance = %d\n", resultCheck.editDistance);
    }
    edlibFreeAlignResult(resultCheck);

    cout << "Input string length: " << xLen << endl;
    cout << "Result diff: " << (result - resultCheck.editDistance) << " (0 for valid execution)" << endl;

    if ((result - resultCheck.editDistance) == 0) {
        cout << "***** PASSED" << endl;
    } else {
        cout << "********** FAILED  " << endl;
    }
    return 0;
}
