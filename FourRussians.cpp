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

    blocks.reserve((unsigned long) numBlocksPerRow + 1);
    table.reserve((unsigned long) (T + 1) * (T + 1));

    constBC.reserve((unsigned long) T);

    for (int i = 0; i < T; i++) {
        constBC.push_back(+1);
    }

    for (int i = 0; i < (T + 1) * (T + 1); i++) {
        table.push_back(0);
    }
}

unsigned long FourRussians::calculate() {

    vector<char> *currentC, *currentB;

    currentC = &constBC;
    currentB = &constBC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {

        blocks.push_back(getTableBlock(currentB, currentC, &X[T * col], &Y[0], T, T));
        currentC = &(blocks.at((unsigned long) col)->lastColumn);
    }
        // last column in first row if any
    if (modX != 0) {
        blocks.push_back(getTableBlock(currentB, currentC, &X[T * numBlocksPerRow], &Y[0], modX, T));
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

    for (auto &block : blocks) {
        for (unsigned long j = 0; j < block->lastRow.size(); j++) {
            result += block->lastRow.at(j);
        }
    }

    result += yLen;
    return result;
}

void FourRussians::calculateRow(int index, unsigned char yLength) {

    vector<char> *currentC, *currentB;
    currentC = &constBC;

    for (int col = 0; col < numBlocksPerRow; col++) {

        currentB = &(blocks.at((unsigned int) col)->lastRow);
        blocks.at((unsigned int) col) = getTableBlock(currentB, currentC, &X[T * col], &Y[T * index], T, yLength);
        currentC = &(blocks.at((unsigned long) col)->lastColumn);
    }

    // last column in row if any
    if (modX != 0) {
        currentB = &(blocks.at((unsigned int) numBlocksPerRow)->lastRow);
        blocks.at((unsigned int) numBlocksPerRow) = getTableBlock(currentB, currentC, &X[T * numBlocksPerRow], &Y[T * index], modX, yLength);
    }
}

Block* FourRussians::getTableBlock(vector<char> *b, vector<char> *c,
        string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {

    Block blk = {*b, *c, getXY(x, y, xLength, yLength)};

    auto foundBlock = generatedBlocks.find(blk);

    if (foundBlock == generatedBlocks.end()) {
        calculateBlock(blk, xLength, yLength);
        generatedBlocks.insert({blk, blk});
        return &(generatedBlocks.find(blk)->second);
    } else {
        found++;
        return &(foundBlock->second);
    }
}

vector<char> FourRussians::getXY(string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {

    char valT = 0, valG = 0, valC = 0, valA = 0, valEmpty = 0;
    unsigned char number = 1;

    vector<char> XY;
    XY.reserve(T * 2);

    for (string::size_type i = 0; i < xLength; ++i) {

        switch (x[i]) {
            case 'A':
                if (valA == 0) {
                    valA = number;
                    number++;
                }
                XY.push_back(valA);
                break;
            case 'T':
                if (valT == 0) {
                    valT = number;
                    number++;
                }
                XY.push_back(valT);
                break;
            case 'G':
                if (valG == 0) {
                    valG = number;
                    number++;
                }
                XY.push_back(valG);
                break;
            case 'C':
                if (valC == 0) {
                    valC = number;
                    number++;
                }
                XY.push_back(valC);
                break;
            case '*':
                if (valEmpty == 0) {
                    valEmpty = number;
                    number++;
                }
                XY.push_back(valEmpty);
                break;
        }
    }

    for (string::size_type i = 0; i < yLength; ++i) {

        switch (y[i]) {
            case 'A':
                if (valA == 0) {
                    valA = number;
                    number++;
                }
                XY.push_back(valA);
                break;
            case 'T':
                if (valT == 0) {
                    valT = number;
                    number++;
                }
                XY.push_back(valT);
                break;
            case 'G':
                if (valG == 0) {
                    valG = number;
                    number++;
                }
                XY.push_back(valG);
                break;
            case 'C':
                if (valC == 0) {
                    valC = number;
                    number++;
                }
                XY.push_back(valC);
                break;
            case '*':
                if (valEmpty == 0) {
                    valEmpty = number;
                    number++;
                }
                XY.push_back(valEmpty);
                break;
        }
    }
    return XY;
}

void FourRussians::calculateBlock(Block &blk,
        unsigned char xLength, unsigned char yLength) {

    char sumB = (char) 0;
    char sumC = (char) 0;

    // convert offset to real and fill the first row
    for (unsigned char i = 1; i <= xLength; i++) {

        sumB += blk.B.at((unsigned long) (i - 1));
        DATA(table, xLength + 1, 0, i) = (char) sumB;
    }
    // convert offset to real and fill the first column
    for (unsigned char i = 1; i <= yLength; i++) {

        sumC += blk.C.at((unsigned long) (i - 1));
        DATA(table, xLength + 1, i, 0) = (char) sumC;
    }

    // TODO: probati implementirati da petlja ide samo do predzadnjeg redka i
    //      i predzadnjeg stupca, zatim zadnji stupac i redak posebno izračunati
    //      i odmah ga ubaciti u lastRow i lastColumn bloka
    for (unsigned char row = 1; row <= yLength; row++) {
        for (unsigned char col = 1; col <= xLength; col++) {

            diagonal = DATA(table, xLength + 1, row - 1, col - 1);
            top = DATA(table, xLength + 1, row - 1, col) + 1;
            left = DATA(table, xLength + 1, row, col - 1) + 1;

            if (blk.XY[col - 1] != blk.XY[xLength + row - 1]) {
                diagonal += 1;
            }
            DATA(table, xLength + 1, row, col) = min(min(diagonal, left), top);
        }
    }

    // last column calculation
    blk.lastColumn.reserve(yLength);

    long first = DATA(table, xLength, 0, xLength);
    long r;
    for (int i = 0; i < yLength; i++) {
        r = DATA(table, xLength + 1, i + 1, xLength);
        blk.lastColumn.push_back((char) (r - first));
        first = r;
    }

    // last row calculation
    blk.lastRow.reserve(xLength);

    first = DATA(table, xLength + 1, yLength, 0);
    for (int i = 0; i < xLength; i++) {
        r = DATA(table, xLength + 1, yLength, i + 1);
        blk.lastRow.push_back((char) (r - first));
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