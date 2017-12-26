#include <iostream>
#include <fstream>
#include <vector>

#include "TableBlock.h"
#include "edlib.h"

#define BLOCK_SIZE 4
#define DUMP_PATH "/tmp/bioinfo/dump.bin"
#define DEFAULT_BC_INDEX_4 6560UL
#define DEFAULT_B_OR_C 80UL
#define ROW_DUMP_PATH "/home/loki/dumpRow.bin"
#define COL_DUMP_PATH "/home/loki/dumpCol.bin"
//#define DEFAULT_FILENAME "/home/loki/CLionProjects/input.txt" // SIZE = 40000
//#define DEFAULT_FILENAME "/home/loki/CLionProjects/input_baseline.txt" // SIZE = 100000
//#define DEFAULT_FILENAME "/home/loki/CLionProjects/inputSimple.txt" // SIZE = 8
#define DEFAULT_FILENAME "/home/loki/CLionProjects/new_input.txt" // SIZE = 10 ** 6


int main(int argc, char** argv) {

    ////////////////////////////////////////
    /////////////// input //////////////////
    ////////////////////////////////////////

    //unsigned char blockSize = 4; //TODO: beware, as this is hardcoded to be 4. It WON'T work with anything else

    //input strings
    string inputFilename, X, Y;

    // user input parsing
    if (argc != 2) {
        inputFilename = DEFAULT_FILENAME;
    } else {
        inputFilename = argv[1];
    }

    // read from file
    ifstream inputFile(inputFilename);
    if (inputFile) {
        getline(inputFile, X);
        getline(inputFile, Y);
    } else {
        cout << "Unable to open file";
        return -1;
    }

    ////////////////////////////////////////
    /////////////// setup //////////////////
    ////////////////////////////////////////

    // start measuring time
    auto t1 = chrono::high_resolution_clock::now();

    unsigned long xLen = X.size();
    unsigned long yLen = Y.size();

    // load cache
    //vector<uint8_t>* lastRowCache;
    //vector<uint8_t>* lastColCache;
    //lastRowCache = loadCache(ROW_DUMP_PATH, blockSize);
    //lastColCache = loadCache(COL_DUMP_PATH, blockSize);

    // a faster cache
    uint8_t* lastRowCacheFast;
    uint8_t* lastColCacheFast;
    lastRowCacheFast = loadCacheToArray4(ROW_DUMP_PATH);
    lastColCacheFast = loadCacheToArray4(COL_DUMP_PATH);

    // check if elements are the same..
//    for(unsigned long i = 1000; i < 1100; i++){
//        cout << lastRowCache->at(i) << "  vs  " << lastRowCacheFast[i] << endl;
//        cout << lastColCache->at(i) << "  vs  " << lastColCacheFast[i] << endl;
//        cout << endl;
//    }
//    cin.ignore();

    unsigned long numLettersToAddX = xLen % BLOCK_SIZE; // just substract from result
    unsigned long numLettersToAddY = yLen % BLOCK_SIZE; // remaining to calculate the old-fashioned way. For now..
    unsigned long numBlocksPerRow  = xLen / BLOCK_SIZE;
    unsigned long numBlocksPerCol  = yLen / BLOCK_SIZE;

//    vector<char> hRow, vRow;
//    hRow.reserve(numBlocksPerCol);
//    vRow.reserve(numBlocksPerCol);

    auto hRowFaster = new char[numBlocksPerCol];
    auto vRowFaster = new char[numBlocksPerCol];

    //vector<unsigned long> xIndexes, yIndexes;
    //xIndexes.reserve(xLen / BLOCK_SIZE);
    //yIndexes.reserve(yLen / BLOCK_SIZE);

    auto xIndexesFaster = new unsigned long[xLen / BLOCK_SIZE];
    auto yIndexesFaster = new unsigned long[yLen / BLOCK_SIZE];

    const char *Xs;
    const char *Ys;

    Xs = X.data();
    Ys = Y.data();

    ////////////////////////////////////////
    ////// precomputing acgt indexes ///////
    ////////////////////////////////////////


    for(unsigned int i = 0; i < (xLen/BLOCK_SIZE); i++){
        //xIndexes.push_back(acgtIndex4(&Xs[i*BLOCK_SIZE]) << 8);
        xIndexesFaster[i] = acgtIndex4(&Xs[i*BLOCK_SIZE]) << 8;
    }

    for(unsigned int i = 0; i < (yLen/BLOCK_SIZE); i++){
        //yIndexes.push_back(acgtIndex4(&Ys[i*BLOCK_SIZE]));
        yIndexesFaster[i] = acgtIndex4(&Ys[i*BLOCK_SIZE]);
    }


    ////////////////////////////////////////
    /////////////// dynamic table //////////
    ////////////////////////////////////////


    /// first block initialization
    //unsigned long fasterIndex = lookUpIndex4(combineXY(xIndexes.at(0), yIndexes.at(0)), DEFAULT_BC_INDEX_4);
    unsigned long fasterIndex = lookUpIndex4(combineXY(xIndexesFaster[0], yIndexesFaster[0]), DEFAULT_BC_INDEX_4);

    uint8_t currentLastRowDense;
    uint8_t currentLastColDense;

    currentLastRowDense = lastRowCacheFast[fasterIndex];//lastRowCache->at(fasterIndex);
    currentLastColDense = lastColCacheFast[fasterIndex];//->at(fasterIndex);

    //vRow.push_back(currentLastColDense);
    //hRow.push_back(currentLastRowDense);
    vRowFaster[0] = currentLastColDense;
    hRowFaster[0] = currentLastRowDense;

    /// initialization of the rest of row
    unsigned long row = 0;
    for(unsigned col = 1; col < numBlocksPerCol; col++){

//        fasterIndex = lookUpIndex4(combineXY(xIndexes.at(col), yIndexes.at(row)),
//                                   //combineBC(vRow.at(col - 1), DEFAULT_B_OR_C));
//                                   combineBC(vRowFaster[col - 1], DEFAULT_B_OR_C));

        fasterIndex = lookUpIndex4(combineXY(xIndexesFaster[col], yIndexesFaster[row]),
                                   combineBC(vRowFaster[col - 1], DEFAULT_B_OR_C));


                //TODO: what just happened?!
        currentLastRowDense = lastColCacheFast[fasterIndex];//lastColCache->at(fasterIndex);
        currentLastColDense = lastRowCacheFast[fasterIndex];//lastRowCache->at(fasterIndex);

//        vRow.push_back(currentLastColDense);
//        hRow.push_back(currentLastRowDense);
        vRowFaster[col] = currentLastColDense;
        hRowFaster[col] = currentLastRowDense;
    }


    /// initialization of the rest of rows
    for(row = 1; row < numBlocksPerRow; row++){

        /// first column initialization
        unsigned long col = 0;
//        fasterIndex = lookUpIndex4(combineXY(xIndexes.at(col), yIndexes.at(row)),
//                                   //combineBC(DEFAULT_B_OR_C, hRow.at(col)));
//                                   combineBC(DEFAULT_B_OR_C, hRowFaster[col]));

        fasterIndex = lookUpIndex4(combineXY(xIndexesFaster[col], yIndexesFaster[row]),
                                   combineBC(DEFAULT_B_OR_C, hRowFaster[col]));

        // TODO: what?! again?
        currentLastRowDense = lastColCacheFast[fasterIndex];//lastColCache->at(fasterIndex);
        currentLastColDense = lastRowCacheFast[fasterIndex];//lastRowCache->at(fasterIndex);

        //hRow.at(col) = currentLastRowDense;
        //vRow.at(col) = currentLastColDense;
        hRowFaster[col] = currentLastRowDense;
        vRowFaster[col] = currentLastColDense;

        /// calculatig values for the rest of columns
        for(col = 1; col < numBlocksPerCol; col++){
            fasterIndex = lookUpIndex4(combineXY(xIndexesFaster[col], yIndexesFaster[row]),
                                       //combineBC(vRow.at(col - 1), hRow.at(col)));
                                       combineBC(vRowFaster[col - 1], hRowFaster[col]));

            // TODO: damn, f*$#k this !?!
            currentLastRowDense = lastColCacheFast[fasterIndex];//lastColCache->at(fasterIndex);
            currentLastColDense = lastRowCacheFast[fasterIndex];//lastRowCache->at(fasterIndex);

//            hRow.at(col) = currentLastRowDense;
//            vRow.at(col) = currentLastColDense;
            hRowFaster[col] = currentLastRowDense;
            vRowFaster[col] = currentLastColDense;
            }

        }

    // TODO: additional rows initialization...

    ////////////////////////////////////////
    ////////// result calculation //////////
    ////////////////////////////////////////

    unsigned long resultFaster = yLen;
    auto lastDPRow = new vector<char>(4);

    for(unsigned int i = 0; i < numBlocksPerRow; i++){
//        unpack4(hRow.at(i), lastDPRow, BLOCK_SIZE);
        unpack4(hRowFaster[i], lastDPRow, BLOCK_SIZE);
        for(auto j: *lastDPRow){
            resultFaster += j;
        }
    }

    // TODO: add a simple cacluation of remaining rows (the ones not divisible by blocksize)
    // TODO: it could be done in O(1) by just using lastDPRow and some substraction...

    // stop measuring
    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;
    cout << "( my ) finished in " << duration  << " seconds, result : " << resultFaster << endl;


    //////////////////////////////////////////////
    /////////// check with edlib /////////////////
    //////////////////////////////////////////////

    t1 = chrono::high_resolution_clock::now();
    EdlibAlignResult resultCheck = edlibAlign(Xs, (int) xLen, Ys, (int) yLen, edlibDefaultAlignConfig());
    if (resultCheck.status == EDLIB_STATUS_OK) {
        t2 = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;
        cout << " edlib finished in " << duration << " seconds, result : " << resultCheck.editDistance << endl;
    }
    edlibFreeAlignResult(resultCheck);

    cin.ignore();


    /////////////////////////////////////////////
    /////////////// free some memory ////////////
    /////////////////////////////////////////////



    return 0;
}