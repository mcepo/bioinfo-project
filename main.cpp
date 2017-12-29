#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

#include "sys/types.h"
#include "sys/sysinfo.h"

#include "FourRussians.h"
#include "edlib/edlib.h"

#define DEFAULT_FILENAME "input.txt"

int main(int argc, char** argv) {

    struct sysinfo memInfo;
    int blockSize = BLOCK_SIZE;

    // start stopwatch
    clock_t start = clock();

    // read strings from input file
    string inputFilename, X, Y;

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
        cout << "Unable to open file" << endl;
        return -1;
    }


    // start memory calculator
    sysinfo(&memInfo);
    long long memBefore = memInfo.totalram - memInfo.freeram;
    memBefore += memInfo.totalswap - memInfo.freeswap;
    memBefore *= memInfo.mem_unit;

    // initialize the algorithm
    FourRussians fr = FourRussians(X, Y, blockSize);
    // calculate edit distance
    unsigned long result = fr.calculate();

    // get memory consumptions
    sysinfo(&memInfo);
    long long memAfter = memInfo.totalram - memInfo.freeram;
    memAfter += memInfo.totalswap - memInfo.freeswap;
    memAfter *= memInfo.mem_unit;

    double memUsage = (((double) ((memAfter - memBefore) / 1024) / 1024));

    const char * X_char = fr.X.c_str();
    const char * Y_char = fr.Y.c_str();

    double execTime = (clock() - start) / (double) CLOCKS_PER_SEC;

    // echo some statistics
    cout << "Input string length: " << fr.xLen << endl;
   // cout << "Total number of blocks in matrix: " << (uint64_t) (fr.numBlocksPerRow * fr.numRowsToCalculate) << endl;
//    cout << "Calculated (stored) blocks: " << fr.calculated << endl;
//    cout << "Found blocks: " << fr.found << endl;
    cout << "Memory used: " << memUsage << "MB" << endl;
    cout << "RESULT: " << result << " generated in: " << execTime << "sec" << endl;

    // EDLIB controle

    if (fr.xLen != 0 && fr.yLen != 0) {
        start = clock();
        EdlibAlignResult resultCheck = edlibAlign(X_char, fr.xLen, Y_char, fr.yLen, edlibDefaultAlignConfig());
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
