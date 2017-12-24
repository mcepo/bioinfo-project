#include <iostream>
#include <fstream>
#include <vector>

#include "TableBlock.h"
#include "edlib.h"


#define DUMP_PATH "/tmp/bioinfo/dump.bin"
#define DEFAULT_BC_INDEX_4 6560UL
#define DEFAULT_B_OR_C 80UL

#define DEFAULT_FILENAME "/home/loki/CLionProjects/input.txt" // SIZE = 40000
//#define DEFAULT_FILENAME "/home/loki/CLionProjects/input_baseline.txt" // SIZE = 100000
//#define DEFAULT_FILENAME "/home/loki/CLionProjects/inputSimple.txt" // SIZE = 8


int main(int argc, char** argv) {

    ////////////////////////////////////////
    /////////////// input //////////////////
    ////////////////////////////////////////

    unsigned char blockSize = 4; //TODO: beware, as this is hardcoded to be 4. It WON'T work with anything else

    //input strings
    string inputFilename, X, Y;

    vector<char>* defaultRowCol;
    vector<char> rc;
    rc.reserve(blockSize);
    for(unsigned int i = 0; i < blockSize; i++){
        rc.push_back((char) +1);
    }
    defaultRowCol = &rc;


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
    vector<uint8_t>* lastRowCache;
    vector<uint8_t>* lastColCache;
    lastRowCache = loadCache("/home/loki/dumpRow.bin", blockSize);
    lastColCache = loadCache("/home/loki/dumpCol.bin", blockSize);

    unsigned long numLettersToAddX = xLen % blockSize; // just substract from result
    unsigned long numLettersToAddY = yLen % blockSize; // remaining to calculate the old-fashioned way. For now..
    unsigned long numBlocksPerRow  = xLen / blockSize;
    unsigned long numBlocksPerCol  = yLen / blockSize;

    vector<char> hRow, vRow;
    hRow.reserve(numBlocksPerCol);
    vRow.reserve(numBlocksPerCol);

    //vector<blockOutput*> blockRow;
    //blockRow.reserve(numBlocksPerCol);

    //vector<unsigned long> indexRow;
    //indexRow.reserve(numBlocksPerCol);

    vector<unsigned long> xIndexes, yIndexes;
    xIndexes.reserve(xLen / blockSize);
    yIndexes.reserve(yLen / blockSize);

    //vector<char> B, C;

    const char *Xs;
    const char *Ys;

    Xs = X.data();
    Ys = Y.data();

    ////////////////////////////////////////
    ////// precomputing acgt indexes ///////
    ////////////////////////////////////////

    //unsigned long defaultBC = combineBC(DEFAULT_BC_INDEX_4, DEFAULT_BC_INDEX_4);


    for(unsigned int i = 0; i < (xLen/blockSize); i++){
        xIndexes.push_back(acgtIndex4(&Xs[i*blockSize]) << 8);
    }

    for(unsigned int i = 0; i < (yLen/blockSize); i++){
        yIndexes.push_back(acgtIndex4(&Ys[i*blockSize]));
    }


    ////////////////////////////////////////
    /////////////// dynamic table //////////
    ////////////////////////////////////////


    /// first block initialization
    //unsigned long currentIndex = lookUpIndex(blockSize, Xs, Ys, defaultRowCol, defaultRowCol);
    unsigned long fasterIndex = lookUpIndex4(combineXY(xIndexes.at(0), yIndexes.at(0)), DEFAULT_BC_INDEX_4);

    //if(currentIndex != fasterIndex){
    //    cout << "err";
    //    cin.ignore();
    //}

    uint8_t currentLastRowDense;
    uint8_t currentLastColDense;

    currentLastRowDense = lastRowCache->at(fasterIndex);//at(currentIndex);
    currentLastColDense = lastColCache->at(fasterIndex);//at(currentIndex);

    vRow.push_back(currentLastColDense);
    hRow.push_back(currentLastRowDense);

    //auto currentLastRow = new vector<char>;
    //auto currentLastCol = new vector<char>;
    //currentLastRow->reserve(blockSize);
    //currentLastCol->reserve(blockSize);

    //unpack4push(currentLastRowDense, currentLastRow, blockSize);
    //unpack4push(currentLastColDense, currentLastCol, blockSize);

    //auto currentResult = new blockOutput;

    //currentResult->H = currentLastRow;
    //currentResult->V = currentLastCol;


    //blockRow.push_back(currentResult);

    /// initialization of the rest of row
    unsigned long row = 0;
    for(unsigned col = 1; col < numBlocksPerCol; col++){

        //vector<char>* lastRes = (blockRow.at(col - 1)->V);

        //vector<char> lastResultVertical = *lastRes;
        //currentIndex = lookUpIndex(blockSize, &Xs[col * blockSize], Ys, defaultRowCol, lastRes);
        fasterIndex = lookUpIndex4(combineXY(xIndexes.at(col), yIndexes.at(row)),
                                   combineBC(vRow.at(col - 1), DEFAULT_B_OR_C));

        //auto firstPart = combineXY(xIndexes.at(col), yIndexes.at(row));
        //auto secondPart = combineBC(vRow.at(col - 1), DEFAULT_B_OR_C);

        //if(currentIndex != fasterIndex){
        //    cout << "err";
        //    cin.ignore();
        //}

        //TODO: what just happened?!
        currentLastRowDense = lastColCache->at(fasterIndex);//at(currentIndex);
        currentLastColDense = lastRowCache->at(fasterIndex);//at(currentIndex);

        vRow.push_back(currentLastColDense);
        hRow.push_back(currentLastRowDense);

        //auto currentLastRow2 = new vector<char>;
        //auto currentLastCol2 = new vector<char>;
        //currentLastRow2->reserve(blockSize);
        //currentLastCol2->reserve(blockSize);

        //unpack4push(currentLastRowDense, currentLastRow2, blockSize);
        //unpack4push(currentLastColDense, currentLastCol2, blockSize);

        //auto currentResult2 = new blockOutput;
        //currentResult2->H = currentLastRow2;
        //currentResult2->V = currentLastCol2;

        //blockRow.push_back(currentResult2);
    }


    /// initialization of the rest of rows
    for(row = 1; row < numBlocksPerRow; row++){

        /// first column initialization
        unsigned long col = 0;

        //vector<char>* lastResH = blockRow.at(col)->H;

        //currentIndex = lookUpIndex(blockSize, &Xs[col * blockSize], &Ys[row * blockSize], lastResH, defaultRowCol);
        fasterIndex = lookUpIndex4(combineXY(xIndexes.at(col), yIndexes.at(row)),
                                   combineBC(DEFAULT_B_OR_C, hRow.at(col)));

//        auto firstPart = combineXY(xIndexes.at(col), yIndexes.at(row));
//        auto secondPart = combineBC(vRow.at(col - 1), DEFAULT_B_OR_C);


        //if(currentIndex != fasterIndex){
        //    cout << "err";
        //    cin.ignore();
        //}

        // TODO: what?! again?
        currentLastRowDense = lastColCache->at(fasterIndex);//at(currentIndex);
        currentLastColDense = lastRowCache->at(fasterIndex);//at(currentIndex);

        hRow.at(col) = currentLastRowDense;
        vRow.at(col) = currentLastColDense;

        // now, use existing reserved memory:
        //currentLastRow = lastResH;
        //currentLastCol = blockRow.at(col)->V;

        //unpack4(currentLastRowDense, currentLastRow, blockSize);
        //unpack4(currentLastColDense, currentLastCol, blockSize);

        /// calculatig values for the rest of columns
        for(col = 1; col < numBlocksPerCol; col++){
            //vector<char>* lastResH2 = blockRow.at(col)->H;
            //vector<char>* lastResV2 = blockRow.at(col - 1)->V;


            //currentIndex = lookUpIndex(blockSize, &Xs[col * blockSize], &Ys[row * blockSize], lastResH2, lastResV2);
            fasterIndex = lookUpIndex4(combineXY(xIndexes.at(col), yIndexes.at(row)),
                                       combineBC(vRow.at(col - 1), hRow.at(col)));

            //if(currentIndex != fasterIndex){
            //    cout << "err";
            //    cin.ignore();
            //}

            // TODO: damn, f*$#k this !?!
            currentLastRowDense = lastColCache->at(fasterIndex);//at(currentIndex);
            currentLastColDense = lastRowCache->at(fasterIndex);//at(currentIndex);

            hRow.at(col) = currentLastRowDense;
            vRow.at(col) = currentLastColDense;

            //currentLastRow = lastResH2;
            //currentLastCol = blockRow.at(col)->V;

            //unpack4(currentLastRowDense, currentLastRow, blockSize);
            //unpack4(currentLastColDense, currentLastCol, blockSize);
            }

        }

    // TODO: additional rows initialization...

    ////////////////////////////////////////
    ////////// result calculation //////////
    ////////////////////////////////////////

    //unsigned long result2 = yLen;
    unsigned long resultFaster = yLen;

    //for(unsigned int i = 0; i < numBlocksPerRow; i++){
    //    result2 += blockRow.at(i)->sum();
    //}

    auto lastDPRow = new vector<char>(4);

    for(unsigned int i = 0; i < numBlocksPerRow; i++){
        unpack4(hRow.at(i), lastDPRow, blockSize);
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

    //cin.ignore();

    return 0;
}