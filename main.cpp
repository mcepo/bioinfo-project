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
    double execTime;

    // start stopwatch
    clock_t start = clock();

// read strings from input file
    string inputFilename = DEFAULT_FILENAME;
    string X, Y;

    if (argc > 1) {
        blockSize = stoi(argv[1]);
    }

    if (argc > 2) {
        inputFilename = argv[2];
    }
    
    ifstream inputFile(inputFilename);

    if (inputFile) {
        getline(inputFile, X);
        getline(inputFile, Y);
    } else {
        cout << "Unable to open file" << endl;
        return -1;
    }

    inputFile.close();

// start memory calculator
    sysinfo(&memInfo);
    long long memBefore = memInfo.totalram - memInfo.freeram;
    memBefore += memInfo.totalswap - memInfo.freeswap;
    memBefore *= memInfo.mem_unit;

// initialize the algorithm
    FourRussians fr = FourRussians(X, Y, blockSize);
    cout << "Block size: " << blockSize << endl;
    cout << "Input string length: " << fr.xLen << endl;

    fr.generateBlocks((blockSize - 1), 0, 0, 0, 0);
    
// calculate edit distance
    cout << "Calculating edit distance ... ";
    unsigned long result = fr.calculateEditDistance();
    execTime = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << result << " in " << execTime << "sec" << endl;
    
// calculate edit script  
    start = clock();
    cout << "Calculating edit script ... ";
    fr.calculateEditScript();
    execTime = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << "DONE in " << execTime << "sec" << endl;

// get memory consumptions
    sysinfo(&memInfo);
    long long memAfter = memInfo.totalram - memInfo.freeram;
    memAfter += memInfo.totalswap - memInfo.freeswap;
    memAfter *= memInfo.mem_unit;
    double memUsage = (((double) ((memAfter - memBefore) / 1024) / 1024));
    cout << "Memory used: " << memUsage << "MB" << endl;
    
// check if result correct
    const char * X_char = fr.X.c_str();
    const char * Y_char = fr.Y.c_str();

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
