#include <iostream>
#include <vector>

#include "TableBlock.h"

int main() {


    char s1[40] = {'a', 'c', 't', 'b', 'g', 'f', 'r', 't', 'z', 'l', 'a', 'c', 't', 'b', 'g', 'f', 'r', 't', 'z', 'l',
                 'a', 'c', 't', 'b', 'g', 'f', 'r', 't', 'z', 'l', 'a', 'c', 't', 'b', 'g', 'f', 'r', 't', 'z', 'l'};

    char s2[38] = {'a', 'c', 'b', 'g', 'f', 'f', 'f', 'r', 't', 'z', 'y', 'a', 'c', 'g', 'y', 'r', 't', 'r', 'a', 'c',
                 'c', 'g', 'f', 'r', 't', 'z', 'l', 'c', 't', 'b', 'f', 'r', 'i', 't', 'z', 'l', 'e', 'k'};

    //distance should be 16

    unsigned char tableDim = 3;
    int seq1Len = 40;
    int seq2Len = 38;

    int numBlocksPerRow = seq1Len / tableDim;
    int numRowsToCalculate = seq2Len / tableDim;
    unsigned char lastBlockColumnCount = tableDim;
    unsigned char lastRowTableSize = tableDim;
    unsigned char lastRowTableCount;
    lastRowTableCount = (unsigned char) numBlocksPerRow;

    if((seq1Len % tableDim) != 0){
        numBlocksPerRow++;
        lastBlockColumnCount = (unsigned char) (seq1Len % tableDim);
    }

    if(seq2Len % tableDim != 0){
        lastRowTableSize = (unsigned char) seq2Len % tableDim;
        lastRowTableCount = (unsigned char) seq1Len / lastRowTableSize;
        numRowsToCalculate += 1;
    }

    unsigned char lastRowLastBlockColumnCount = lastRowTableSize;

    if(seq1Len % lastRowTableSize != 0){
        lastRowLastBlockColumnCount = (unsigned char) seq1Len % lastRowTableSize;
        lastRowTableCount ++;
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

        bool viseNeDiraj = false;

        /// chained
        if((row == (numRowsToCalculate - 1)) && (lastRowTableSize != tableDim)){
            numBlocksPerRow = lastRowTableCount;
            currentB.clear();
            for (auto tmp : blocks) {
                vector<char> tmpChar = tmp.horizontalF(false);
                for (char cc : tmpChar) {
                    currentB.push_back(cc);
                }
            }
            viseNeDiraj = true;
            tableDim = lastRowTableSize;
            blocks.clear();
        }

        for(int col = 0; col < numBlocksPerRow; col ++){

            if(row != 0 && !viseNeDiraj){
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

            for(int i = 0; i < tableDim; i++){
                currentX.push_back(s1[(tableDim * col) + i]);
            }


            if(col == (numBlocksPerRow - 1)){
                if(row != (numRowsToCalculate - 1)){ // zadnji stupac
                    TableBlock blk(tableDim, currentB, currentC, currentX, currentY, 0, lastBlockColumnCount);

                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
                else{ //zadnji red, zadnji stupac
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY, 0, lastRowLastBlockColumnCount);
                    blk.calculate();
                    for(int i = 0; i < lastBlockColumnCount; i++){
                        currentB.erase(currentB.begin());
                    }
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        if(!viseNeDiraj){
                        blocks.at((unsigned int) col) = blk;
                        } else{
                            blocks.push_back(blk);
                        }

                    }
                }
            }
            else{
                if(row != (numRowsToCalculate - 1)){ // neki srednji cell
                    TableBlock blk(tableDim, currentB, currentC, currentX, currentY);
                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        blocks.at((unsigned int) col) = blk;
                    }
                }
                else{ // zadnji red
                    TableBlock blk(lastRowTableSize, currentB, currentC, currentX, currentY);
                    for(int i = 0; i < lastRowTableSize; i++){
                        currentB.erase(currentB.begin());
                    }

                    blk.calculate();
                    if(row == 0) {
                        blocks.push_back(blk);
                    }
                    else{
                        if(!viseNeDiraj){
                            blocks.at((unsigned int) col) = blk;
                        }
                        else{
                            blocks.push_back(blk);
                        }

                    }
                }
            }

            cout << "################" << endl;
            cout << "r: " << row << " c: " << col << endl;

            blocks.at((unsigned long) col).print();
            cout << "V: ";
            currentC = blocks.at((unsigned long) col).verticalF();

            cout << "H: ";
            blocks.at((unsigned long)  col).horizontalF();


        }
    }

    unsigned long result = 0L;

    for (auto &block : blocks) {
        for(unsigned long j = 0; j < block.horizontalF(false).size(); j++){
            result += block.horizontalF(false).at(j);
        }
    }

    result += seq2Len;

    cout << "rows of blocks:" << numRowsToCalculate << " last row size " << (int) lastRowTableSize << endl;

    cout << "result (edit distance): " << result;


    return 0;
}


