#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TableBlock.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"

int main(int argc, char** argv) {

    string inputFilename, X, Y;

    int blockSize = 3;

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
	
	int appendLengthX = blockSize - (X.size() % blockSize);
	int appendLengthY = blockSize - (Y.size() % blockSize);
	
	for(int i = 0; i < appendLengthX; i++) { X += '*'; }
	for(int i = 0; i < appendLengthY; i++) { Y += '*'; }
	
    int xLen = X.size();
    int yLen = Y.size();

    int numBlocksPerRow = xLen / blockSize;
    int numRowsToCalculate = yLen / blockSize;

    vector<TableBlock> blocks;
    blocks.reserve((unsigned long) numBlocksPerRow);

    // the magic

    //int row = 0;
    vector<char> currentY, currentC, currentB, currentX;
    currentX.reserve((unsigned long) blockSize);
    currentB.reserve((unsigned long) blockSize);
    currentY.reserve((unsigned long) blockSize);
    currentC.reserve((unsigned long) blockSize);

    for (int row = 0; row < numRowsToCalculate; row++) {

        cout << "------------------------" << endl;

        currentY.clear();

        //if(currentY.capacity() != blockSize){
        //    cout << "err";
        //}

        for (int i = 0; i < blockSize; i++) {
            char element = Y[(blockSize * row) + i];
            currentY.push_back(element); // ok
        }


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

            currentX.clear();

            for (int i = 0; i < blockSize; i++) {
                currentX.push_back(X[(blockSize * col) + i]);
            }
			
            TableBlock blk(blockSize, currentB, currentC, currentX, currentY);
                blk.calculate();
            if (row == 0) {
				blocks.push_back(blk);
            } else {
                blocks.at((unsigned int) col) = blk;
            }

            cout << "################" << endl;
            cout << "r: " << row << " c: " << col << endl;
            blocks.at((unsigned long) col).print();
            cout << "V: ";
            currentC = blocks.at((unsigned long) col).verticalF();
            cout << "H: ";
            blocks.at((unsigned long) col).horizontalF();
        }
    }

    unsigned long result = 0L;

    for (auto &block : blocks) {
        for (unsigned long j = 0; j < block.horizontalF(false).size(); j++) {
            result += block.horizontalF(false).at(j);
        }
    }

    result += yLen;

    cout << "rows of blocks:" << numRowsToCalculate << endl;
    cout << "Calculated result (edit distance): " << result;

    // EDLIB CONTROL

    const char * X_char = X.c_str();
    const char * Y_char = Y.c_str();

    EdlibAlignResult resultCheck = edlibAlign(X_char, X.size(), Y_char, Y.size(), edlibDefaultAlignConfig());
    if (resultCheck.status == EDLIB_STATUS_OK) {
        printf("\n*********** \n Edlib control check -> edit_distance = %d\n", resultCheck.editDistance);
    }
    edlibFreeAlignResult(resultCheck);

    return 0;
}
