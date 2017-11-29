#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

#include "FourRussians.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"

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

    FourRussians fr = FourRussians(X, Y, blockSize);
    unsigned long result = fr.calculate();

    const char * X_char = fr.X.c_str();
    const char * Y_char = fr.Y.c_str();

    double execTime = (clock() - start) / (double) CLOCKS_PER_SEC;

    cout << "Input string length: " << fr.xLen << endl;
    cout << "Total number of blocks in matrix: " << (fr.numBlocksPerRow * fr.numRowsToCalculate) << endl;
    cout << "Calculated (stored) blocks: " << fr.generatedBlocks.size() << endl;
    cout << "Found blocks: " << fr.found << endl;
    cout << "RESULT: " << result << " generated in: " << execTime << "sec" << endl;

    // EDLIB controle

    if (fr.xLen != 0 && fr.yLen != 0) {
        start = clock();
        EdlibAlignResult resultCheck = edlibAlign(X_char, fr.xLen , Y_char, fr.yLen , edlibDefaultAlignConfig());
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
