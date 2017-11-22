#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "TableBlock.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"

int main(int argc, char** argv) {

    string inputFilename, X, Y;

    int blockSize = 4;

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

    int xLen = X.size();
    int yLen = Y.size();

    int numBlocksPerRow = xLen / blockSize;
    int numRowsToCalculate = yLen / blockSize;
    unsigned char lastBlockColumnCount = blockSize;
    unsigned char lastRowTableSize = blockSize;

    unsigned char lastRowTableCount;
    lastRowTableCount = (unsigned char) numBlocksPerRow;

    if ((xLen % blockSize) != 0) {
        numBlocksPerRow++;
        lastBlockColumnCount = (unsigned char) (xLen % blockSize);
    }

    if (yLen % blockSize != 0) {
        lastRowTableSize = (unsigned char) yLen % blockSize;
        lastRowTableCount = (unsigned char) yLen / lastRowTableSize;
        numRowsToCalculate += 1;
    }

    unsigned char lastRowLastBlockColumnCount = lastRowTableSize;

    if (xLen % lastRowTableSize != 0) {
        lastRowLastBlockColumnCount = (unsigned char) xLen % lastRowTableSize;
        lastRowTableCount++;
    }

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

    //    cout << "------------------------" << endl;

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

        bool viseNeDiraj = false;

        /// chained
        if ((row == (numRowsToCalculate - 1)) && (lastRowTableSize != blockSize)) {
            numBlocksPerRow = lastRowTableCount;
            currentB.clear();
            for (auto tmp : blocks) {
                vector<char> tmpChar = tmp.horizontalF(false);
                for (char cc : tmpChar) {
                    currentB.push_back(cc);
                }
            }
            viseNeDiraj = true;
            blockSize = lastRowTableSize;
            blocks.clear();
        }

        for (int col = 0; col < numBlocksPerRow; col++) {

            if (row != 0 && !viseNeDiraj) {
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


            if (col == (numBlocksPerRow - 1)) {
                if (row != (numRowsToCalculate - 1)) { // zadnji stupac
                    TableBlock blk(blockSize, currentB, currentC, currentX, currentY, 0, lastBlockColumnCount);
                    blk.calculate();
                    if (row == 0) {
                        blocks.push_back(blk);
                    } else {
                        blocks.at((unsigned int) col) = blk;
                    }
                } else { //zadnji red, zadnji stupac
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY, 0, lastRowLastBlockColumnCount);
                    blk.calculate();
                    for (int i = 0; i < lastBlockColumnCount; i++) {
                        currentB.erase(currentB.begin());
                    }
                    if (row == 0) {
                        blocks.push_back(blk);
                    } else {
                        if (!viseNeDiraj) {
                            blocks.at((unsigned int) col) = blk;
                        } else {
                            blocks.push_back(blk);
                        }
                    }
                }
            } else {
                if (row != (numRowsToCalculate - 1)) { // neki srednji cell
                    TableBlock blk(blockSize, currentB, currentC, currentX, currentY);
                    blk.calculate();
                    if (row == 0) {
                        blocks.push_back(blk);
                    } else {
                        blocks.at((unsigned int) col) = blk;
                    }
                } else { // zadnji red
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY);
                    for (int i = 0; i < lastRowTableSize; i++) {
                        currentB.erase(currentB.begin());
                    }

                    blk.calculate();
                    if (row == 0) {
                        blocks.push_back(blk);
                    } else {
                        if (!viseNeDiraj) {
                            blocks.at((unsigned int) col) = blk;
                        } else {
                            blocks.push_back(blk);
                        }

                    }
                }
            }

        //    cout << "################" << endl;
			
        //    cout << "r: " << row << " c: " << col << endl;

        //    blocks.at((unsigned long) col).print();
        //    cout << "V: ";
            currentC = blocks.at((unsigned long) col).verticalF(false);

        //    cout << "H: ";
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

   // cout << "rows of blocks:" << numRowsToCalculate << " last row size " << (int) lastRowTableSize << endl;

  //  cout << "Calculated result (edit distance): " << result;

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
