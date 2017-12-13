//
// Created by loki on 11/4/17.
//

#include "FourRussians.h"

FourRussians::FourRussians() {
}

FourRussians::FourRussians(string x, string y, int blockSize) {

    T = blockSize;
    // TODO: bounds, ranges and sizes check...

    X = (x);
    Y = (y);
    
    found = 0;

    modX = X.size() % blockSize;
    modY = Y.size() % blockSize;

    xLen = X.size();
    yLen = Y.size();

    numBlocksPerRow = xLen / blockSize;
    numRowsToCalculate = yLen / blockSize;

    blocks = new Block[ numBlocksPerRow + 1];
    table = new char[ (T + 1) * (T + 1)];
    table[0] = 0;

    constBC = new char[T];

    for (int i = 0; i < T; i++) {
        constBC[i] = 1;
    }
}

unsigned long FourRussians::calculate() {

    char *currentC, *currentB;

    currentC = constBC;
    currentB = constBC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {
        blocks[col] = getTableBlock(currentB, currentC, &X[T * col], &Y[0], T, T);
        currentC = blocks[col].lastColumn;
    }

    // last column in first row if any
    if (modX != 0) {
        blocks[numBlocksPerRow] = getTableBlock(currentB, currentC, &X[T * numBlocksPerRow], &Y[0], modX, T);
    }

    // rest of the matrix
    for (int row = 1; row < numRowsToCalculate; row++) {
        calculateRow(row, T);
    }

    // last row calculation if any
    if (modY != 0) {
        calculateRow(numRowsToCalculate, modY);
    }

    unsigned long result = 0L;

    for (int i = 0; i <= numBlocksPerRow; i++) {
        for (unsigned long j = 0; j < blocks[i].xLength; j++) {
            result += blocks[i].lastRow[j];
        }
    }

    result += yLen;
    return result;
}

void FourRussians::calculateRow(int index, unsigned char yLength) {


    char *currentC, *currentB;
    currentC = constBC;

    for (int col = 0; col < numBlocksPerRow; col++) {

        currentB = blocks[col].lastRow;
        blocks[col] = getTableBlock(currentB, currentC, &X[T * col], &Y[T * index], T, yLength);
        currentC = blocks[col].lastColumn;
    }

    // last column in row if any
    if (modX != 0) {
        currentB = blocks[numBlocksPerRow].lastRow;
        blocks[numBlocksPerRow] = getTableBlock(currentB, currentC, &X[T * numBlocksPerRow], &Y[T * index], modX, yLength);
    }
}

Block& FourRussians::getTableBlock(char *b, char *c,
        string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {
    
    Block blk = {b, c, getXY(x, y, xLength, yLength), xLength, yLength};
    
    auto foundBlock = generatedBlocks.find(blk);
    
    if (foundBlock == generatedBlocks.end()) {
    
        calculateBlock(blk);
        generatedBlocks.insert({blk, blk});
        return generatedBlocks.find(blk)->second;
    
    } else {
    
        found++;
        return foundBlock->second;
    
    }
}

char* FourRussians::getXY(string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {

    char valT = 0, valG = 0, valC = 0, valA = 0;
    unsigned char number = 1;

    char *XY = new char[T * 2];
    int j = 0;

    for (string::size_type i = 0; i < xLength; ++i, ++j) {

        switch (x[i]) {
            case 'A':
                if (valA == 0) {
                    valA = number;
                    number++;
                }
                XY[j] = valA;
                break;
            case 'T':
                if (valT == 0) {
                    valT = number;
                    number++;
                }
                XY[j] = valT;
                break;
            case 'G':
                if (valG == 0) {
                    valG = number;
                    number++;
                }
                XY[j] = valG;
                break;
            case 'C':
                if (valC == 0) {
                    valC = number;
                    number++;
                }
                XY[j] = valC;
                break;
        }
    }

    for (string::size_type i = 0; i < yLength; ++i, ++j) {

        switch (y[i]) {
            case 'A':
                if (valA == 0) {
                    valA = number;
                    number++;
                }
                XY[j] = valA;
                break;
            case 'T':
                if (valT == 0) {
                    valT = number;
                    number++;
                }
                XY[j] = valT;
                break;
            case 'G':
                if (valG == 0) {
                    valG = number;
                    number++;
                }
                XY[j] = valG;
                break;
            case 'C':
                if (valC == 0) {
                    valC = number;
                    number++;
                }
                XY[j] = valC;
                break;
        }
    }

    return XY;
}

void FourRussians::calculateBlock(Block &blk) {

    char sumB = (char) 0;
    char sumC = (char) 0;

    // convert offset to real and fill the first row
    for (unsigned char i = 1; i <= blk.xLength; i++) {

        sumB += blk.B[i - 1];
        DATA(table, blk.xLength + 1, 0, i) = (char) sumB;
    }
    // convert offset to real and fill the first column
    for (unsigned char i = 1; i <= blk.yLength; i++) {

        sumC += blk.C[i - 1];
        DATA(table, blk.xLength + 1, i, 0) = (char) sumC;
    }

    // TODO: probati implementirati da petlja ide samo do predzadnjeg redka i
    //      i predzadnjeg stupca, zatim zadnji stupac i redak posebno izračunati
    //      i odmah ga ubaciti u lastRow i lastColumn bloka
    for (unsigned char row = 1; row <= blk.yLength; row++) {
        for (unsigned char col = 1; col <= blk.xLength; col++) {

            diagonal = DATA(table, blk.xLength + 1, row - 1, col - 1);
            top = DATA(table, blk.xLength + 1, row - 1, col) + 1;
            left = DATA(table, blk.xLength + 1, row, col - 1) + 1;

            if (blk.XY[col - 1] != blk.XY[blk.xLength + row - 1]) {
                diagonal += 1;
            }
            DATA(table, blk.xLength + 1, row, col) = min(min(diagonal, left), top);
        }
    }

    // last column calculation
    blk.lastColumn = new char[blk.yLength];

    long first = DATA(table, blk.xLength, 0, blk.xLength);
    long r;
    for (int i = 0; i < blk.yLength; i++) {
        r = DATA(table, blk.xLength + 1, i + 1, blk.xLength);
        blk.lastColumn[i] = r - first;
        first = r;
    }

    // last row calculation
    blk.lastRow = new char[blk.xLength];

    first = DATA(table, blk.xLength + 1, blk.yLength, 0);
    for (int i = 0; i < blk.xLength; i++) {
        r = DATA(table, blk.xLength + 1, blk.yLength, i + 1);
        blk.lastRow[i] = r - first;
        first = r;
    }
    //   print(blk, xLength, yLength);
}

void FourRussians::print(Block &blk, unsigned char xLength, unsigned char yLength) {

    cout << "TB" << "| x  ";

    for (unsigned int col = 0; col < xLength; col++) {
        cout << (int) blk.XY[col] << "  ";
    }

    cout << endl << "--+";
    for (unsigned int col = 0; col <= xLength; col++) {
        cout << "---";
    }

    for (unsigned int row = 0; row <= yLength; row++) {
        if (row == 0) {
            cout << endl << "x | ";
        } else {
            cout << endl << (int) blk.XY[xLength + row - 1] << " | ";
        }
        for (unsigned int col = 0; col <= xLength; col++) {
            cout << (int) DATA(table, xLength + 1, row, col) << "  ";
        }
    }
    cout << endl;
}
