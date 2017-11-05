#include <iostream>
#include <vector>

#include "TableBlock.h"

int main() {
//    unsigned long a = 2;
//    unsigned long b_[] = {1, 2, 3};
//    unsigned long c_[] = {3, 3, 4};
//    char x_[] = {'t', 'a', 'g'};
//    char y_[] = {'c', 'a', 'a'};


//    unsigned long b_[] = {1, 2, 3};
//    unsigned long c_[] = {3, 3, 4};
//    char x_[] = {'g', 't', 'a', 'g'};
//    char y_[] = {'t', 'c', 'a', 'a'};

//    unsigned long a = 3;
//    unsigned long b_[] = {4, 5, 4};
//    unsigned long c_[] = {4, 4, 5};
//    char x_[] = {'g', 'c', 't', 'a'};
//    char y_[] = {'a', 'c', 'g', 't'};

    char b_[] = {+1, +1, +1};
    char c_[] = {-1, -1, +1};
    char x_[] = {'t', 'a', 'g'};
    char y_[] = {'t', 'g', 't'};

    int tabl = 3;

    std::vector<char> b, c;
    std::vector<char> x, y;

    for(int i = 0; i < tabl; i++){
        b.push_back(b_[i]);
    }

    for(int i = 0; i < tabl; i++){
        c.push_back(c_[i]);
    }

    for(int i = 0; i < tabl + 1; i++) {
        x.push_back(x_[i]);
    }

    for(int i = 0; i < tabl + 1; i++) {
        y.push_back(y_[i]);
    }

    TableBlock tablica((unsigned char) 3, b, c, x, y, 3, 2);
    tablica.A = 3;
    tablica.calculate();

    //tablica.print();

    //tablica.emptyTemp();
    //tablica.horizontalF();
    //tablica.verticalF();

    ///////////////////////////////////
    ////////// sequence alignment /////
    ///////////////////////////////////

    char s1[] = {'a', 'c', 'g', 't', 'c', 'g', 't', 'a', 'g', 'c', 't'}; //, 'a'};
    char s2[] = {'a', 'c', 'g', 't', 'g', 't', 'c', 'a', 'a', 'c'};//, 'g'}; //, 't'};

    unsigned char tableDim = 3;
    int seq1Len = 11;
    int seq2Len = 10;

    int numBlocksPerRow = seq1Len / tableDim;
    int numRowsToCalculate = seq2Len / tableDim;
    unsigned char lastBlockColumnCount = tableDim;
    unsigned char lastRowTableSize = tableDim;

    if(seq1Len % tableDim != 0){
        numBlocksPerRow++;
        lastBlockColumnCount = (unsigned char) (seq1Len % tableDim);
    }

    if(seq2Len % tableDim != 0){
        lastRowTableSize = (unsigned char) seq2Len % tableDim;
    }

    unsigned char lastRowLastBlockColumnCount = lastRowTableSize;

    if(seq1Len % lastRowTableSize != 0){
        lastRowLastBlockColumnCount = (unsigned char) seq1Len % lastRowTableSize;
    }


    vector<TableBlock> blocks;
    blocks.reserve((unsigned long) numBlocksPerRow);

    vector<char> x__, y__, b__, c__;

    b__.reserve((unsigned long) seq1Len);
    c__.reserve((unsigned long) seq2Len);


    for(int i = 0; i < tableDim; i++){
        b__.push_back(+1);
        c__.push_back(+1);
    }

    // the magic

    //int row = 0;
    vector<char> currentY, currentC, currentB, currentX;
    currentX.reserve((unsigned long) tableDim);
    currentB.reserve((unsigned long) tableDim);
    currentY.reserve((unsigned long) tableDim);
    currentC.reserve((unsigned long) tableDim);

    for(int row = 0; row < numRowsToCalculate; row++){

        cout << "------------------------" << endl;

        currentY.clear();

        //if(currentY.capacity() != tableDim){
        //    cout << "err";
        //}

        for(int i = 0; i < tableDim; i++){
            char element = s2[(tableDim * row) + i];
            currentY.push_back(element); // ok
        }


        if(row == 0){
            for(int i = 0; i < tableDim; i++) {
                currentB.push_back(+1);                         // ok
            }
        }


        /// chained
        for(int col = 0; col < numBlocksPerRow; col ++){

            if(row != 0){
                currentB.clear();
                currentB = blocks.at((unsigned int) col).horizontalF(false);
            }

            if(col == 0){
                currentC.clear();
                for(int i = 0; i < tableDim; i++){ // do dynamicaly
                    currentC.push_back(+1);                     // ok
                }
            }

            currentX.clear();
            if(currentX.capacity() != tableDim){
                cout << "err";
            }

            //if(row != 0){
            //    currentB.clear();
            //    if(currentB.capacity() != tableDim){
            //        cout << "err";
            //    }
            //
            //}


            for(int i = 0; i < tableDim; i++){
                currentX.push_back(s1[(tableDim * col) + i]);
            }


            if(col == (numBlocksPerRow - 1)){
                if(row != (numRowsToCalculate - 1)){
                    TableBlock blk(tableDim, currentB, currentC, currentX, currentY, 0, lastBlockColumnCount);
                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
                else{
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY, 0, lastRowLastBlockColumnCount);
                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
            }
            else{
                if(row != (numRowsToCalculate - 1)){
                    TableBlock blk(tableDim, currentB, currentC, currentX, currentY);
                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
                else{
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY);
                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
            }

            blocks.at((unsigned long) col).print();
            cout << "V: ";
            currentC = blocks.at((unsigned long) col).verticalF();

            cout << "H: ";
            blocks.at((unsigned long)  col).horizontalF();

            //TODO: change C

        }
    }

    unsigned long result = 0L;

    for(unsigned long i = 0; i < blocks.size(); i++){
        for(unsigned long j = 0; j < blocks.at(i).horizontalF(false).size(); j++){
            result += blocks.at(i).horizontalF(false).at(j);
        }
    }

    result += ((numRowsToCalculate - 1) * tableDim) + lastRowTableSize;

    cout << "rows of blocks:" << numRowsToCalculate << " last row size " << (int) lastRowTableSize << endl;

    cout << "result (edit distance): " << result;


    return 0;
}


