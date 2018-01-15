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


// return amount of current memory consumed

long long currentMemory() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    return ((memInfo.totalram - memInfo.freeram)
            + (memInfo.totalswap - memInfo.freeswap)
            * memInfo.mem_unit);
}

// calculate memory difference

double memoryDiff(long long memoryStart) {
    return (((double) ((currentMemory() - memoryStart) / 1024) / 1024));
}

int main(int argc, char** argv) {

    // start stopwatch
    clock_t start = clock();
    // start memory calculator
    long long totalMemoryBefore = currentMemory();

    int blockSize = BLOCK_SIZE;
    double execTime;

    // read strings from input file
    string inputFilename = DEFAULT_FILENAME;
    string X, Y;

    // input arguments: T inputFilePath OutputFilePath

    // check user input
    if (argc != 4 || (((argv[1][0] < '0') && (argv[1][0] > '9')))){
        cout << "invalid parameters entered!\n\nusage:\n./fourRussians T inputFilePath outputFilePath" << endl << endl;
        cout << "T             : caching block size (T, T). 2 to 4 is preferred" << endl << endl;
        cout << "inputFilePath : input file path. File should have two rows," << endl;
        cout << "                with a single sequence in each line" << endl << endl;
        cout << "OutputFilePath: output file containing edit script (in MAF format)," << endl;
        cout << "                using Levenshtein (edit) distance metric" << endl;
        exit(-1);
    }

    //if (argc > 1) {
    string t_input = "";
    t_input += argv[1][0];
    blockSize = stoi(t_input);
    //}

    inputFilename = argv[2];

    string outputFilename = argv[3];

    ifstream inputFile(inputFilename);

    if (inputFile) {
        getline(inputFile, X);
        getline(inputFile, Y);
    } else {
        cout << "Unable to open file" << endl;
        return -1;
    }

    inputFile.close();

    // initialize the algorithm
    FourRussians fr = FourRussians(X, Y, blockSize);
    cout << "Input X length: " << fr.xLen - fr.xMod << endl;
    cout << "Input Y length: " << fr.yLen - fr.yMod << endl;
    cout << "Used block size: " << (int) fr.T << endl;
    cout << "Optimal block size: " << (int) fr.optimalT << endl;

    cout << "\tAction\t\tRuntime\t\tMemory" << endl;

    // precalculate blocks, measure time and memory during
    clock_t timeBeforeBlockCalc = clock();
    long long memoryBeforeBlockCalc = currentMemory();

    fr.generateBlocks((blockSize - 1), 1, 1, 0, 0, 0, 0);

    execTime = (clock() - timeBeforeBlockCalc) / (double) CLOCKS_PER_SEC;
    cout << "Generating blocks - \t" << execTime << " sec"
            << "\t" << memoryDiff(memoryBeforeBlockCalc) << " MB" << endl;

    // calculate edit distance, measure time and memory during
    clock_t timeBeforeMatrix = clock();
    long long memoryBeforeMatrix = currentMemory();

    fr.calculateMatrix();

    execTime = (clock() - timeBeforeMatrix) / (double) CLOCKS_PER_SEC;
    cout << "Calculating matrix - \t" << execTime << " sec"
            << "\t" << memoryDiff(memoryBeforeMatrix) << " MB" << endl;

    // calculate edit script, measure time and memory during
    clock_t timeBeforeEditScript = clock();
    long long memoryBeforeEditScript = currentMemory();

    unsigned long result = fr.calculateEditDistanceAndScript(outputFilename);

    execTime = (clock() - timeBeforeEditScript) / (double) CLOCKS_PER_SEC;
    cout << "Edit script&distance - \t" << execTime << " sec"
            << "\t" << memoryDiff(memoryBeforeEditScript) << " MB" << endl;

    // echo total runtime and memory
    execTime = (clock() - start) / (double) CLOCKS_PER_SEC;
    cout << endl << "Total- \t\t\t" << execTime << "sec"
            << "\t" << memoryDiff(totalMemoryBefore) << "MB" << endl;

    cout << endl << "Result:\t" << result << endl;
    
    cout << endl << "Edlib control ";

    // EDLIB check
    const char * X_char = fr.X.c_str();
    const char * Y_char = fr.Y.c_str();

    if (fr.xLen != 0 && fr.yLen != 0) {
        start = clock();
        EdlibAlignResult resultCheck = edlibAlign(X_char, (fr.xLen - fr.xMod) , Y_char,
                                                  (fr.yLen - fr.yMod) , edlibDefaultAlignConfig());
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
    cout << endl;
    return 0;
}

