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
    calculated = 0;

    modX = X.size() % blockSize;
    modY = Y.size() % blockSize;

    xLen = X.size();
    yLen = Y.size();

    numBlocksPerRow = xLen / blockSize;
    numRowsToCalculate = yLen / blockSize;

    unsigned long numComb = numCombinations(T);

    blocks = new char*[ numBlocksPerRow + 1];
    generatedBlocks = new char*[numComb];

    cout << "Total number of blocks: " << numComb << endl;

    double mem = (numComb * (2 * T)) / 1024 / 1024;

    cout << "Expected memory usage: " << mem << "MB" << endl;

    table = new char[ (T + 1) * (T + 1)];
    table[0] = 0;

    constBC = new char[T];

    for (int i = 0; i < T; i++) {
        constBC[i] = 1;
    }
}

unsigned long FourRussians::calculate() {

    char *currentC;

    currentC = constBC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {
        blocks[col] = getTableBlock(constBC, currentC, &X[T * col], &Y[0], T, T);
        //lastColumn
        currentC = &blocks[col][0];
    }

    // last column in first row if any
    if (modX != 0) {
        blocks[numBlocksPerRow] = getTableBlock(constBC, currentC, &X[T * numBlocksPerRow], &Y[0], modX, T);
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

    for (int i = 0; i < numBlocksPerRow; i++) {
        for (unsigned long j = 0; j < T; j++) {
            result += blocks[i][T + j];
        }
    }

    if (modX != 0) {
        for (unsigned long j = 0; j < modX; j++) {
            result += blocks[numBlocksPerRow][T + j];
        }
    }

    result += yLen;
    return result;
}

void FourRussians::calculateRow(int index, unsigned char yLength) {


    char *currentC, *currentB;
    currentC = constBC;

    for (int col = 0; col < numBlocksPerRow; col++) {

        //lastRow
        currentB = &blocks[col][T];
        blocks[col] = getTableBlock(currentB, currentC, &X[T * col], &Y[T * index], T, yLength);
        //lastColumn
        currentC = &blocks[col][0];
    }

    // last column in row if any
    if (modX != 0) {
        //lastRow
        currentB = &blocks[numBlocksPerRow][T];
        blocks[numBlocksPerRow] = getTableBlock(currentB, currentC, &X[T * numBlocksPerRow], &Y[T * index], modX, yLength);
    }
}

char* FourRussians::getTableBlock(char *b, char *c,
        string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {

    unsigned long index = lookUpIndex(xLength, yLength, x, y, b, c);

    if (generatedBlocks[index] == 0) {
        generatedBlocks[index] = calculateBlock(b, c, x, y, xLength, yLength);
        calculated++;
    } else {
        found++;
    }
    return generatedBlocks[index];
}

char* FourRussians::calculateBlock(char *b, char *c,
        string const& x, string const& y,
        unsigned char xLength, unsigned char yLength) {

    // convert offset to real and fill the first row
    for (unsigned char i = 1; i <= xLength; i++) {

        DATA(table, xLength + 1, 0, i) = DATA(table, xLength + 1, 0, i - 1) + b[i - 1];
    }
    // convert offset to real and fill the first column
    for (unsigned char i = 1; i <= yLength; i++) {

        DATA(table, xLength + 1, i, 0) = DATA(table, xLength + 1, i - 1, 0) + c[i - 1];
    }

    // TODO: probati implementirati da petlja ide samo do predzadnjeg redka i
    //      i predzadnjeg stupca, zatim zadnji stupac i redak posebno izračunati
    //      i odmah ga ubaciti u lastRow i lastColumn bloka
    for (unsigned char row = 1; row <= yLength; row++) {
        for (unsigned char col = 1; col <= xLength; col++) {

            diagonal = DATA(table, xLength + 1, row - 1, col - 1);
            top = DATA(table, xLength + 1, row - 1, col) + 1;
            left = DATA(table, xLength + 1, row, col - 1) + 1;

            if (x[col - 1] != y[row - 1]) {
                diagonal += 1;
            }
            DATA(table, xLength + 1, row, col) = min(min(diagonal, left), top);
        }
    }

    char *F = new char[yLength + xLength];

    // last column calculation
    long first = DATA(table, xLength, 0, xLength);
    long r;
    for (int i = 0; i < yLength; i++) {
        r = DATA(table, xLength + 1, i + 1, xLength);
        F[i] = r - first;
        first = r;
    }

    // last row calculation

    first = DATA(table, xLength + 1, yLength, 0);
    for (int i = 0; i < xLength; i++) {
        r = DATA(table, xLength + 1, yLength, i + 1);
        F[i + yLength] = r - first;
        first = r;
    }
    return F;
}

unsigned int FourRussians::acgt_to_index(const char acgt) {
    if (acgt == 'A') return 0;
    if (acgt == 'C') return 1;
    if (acgt == 'G') return 2;
    if (acgt == 'T') return 3;
}

uint8_t compact(vector<char>* element, unsigned char T) { // for t == 3! // be extra carefull, as this will overflow for t > 4
    uint8_t result = 0;
    uint8_t decimal = 1;

    for (auto i = (unsigned long) T; i > 0; i--) {
        auto e = (uint8_t) (element->at(i - 1) + (char) 1); // normalize it from {-1, 0, 1} to {0, 1, 2}
        result += (e * decimal);
        decimal *= 3;
    }

    return result;
}

void unpack(uint8_t dense, vector<char>* out, unsigned char T) { // this will also overflow for T > 4...

    auto decimal = uint8_t(pow(3, T - 1) + ((double) 0.5)); //TODO: write this more efficiently (for loop?)
    for (unsigned long i = 0; i < T; i++) {
        auto r = dense / decimal;
        dense -= r * decimal;
        auto result = (((char) r) - ((char) 1));
        out->at(i) = result;
        decimal /= 3;
    }
}

char FourRussians::index_to_acgt(unsigned int index) {
    if (index == 0) return 'A';
    if (index == 1) return 'C';
    if (index == 2) return 'G';
    if (index == 3) return 'T';
    return -1;
}

unsigned long FourRussians::numCombinations(unsigned char T) {
    return (unsigned long) lround(pow(3, 2 * T) * pow(4, 2 * T));
}

unsigned int FourRussians::bc_to_index(char bc) {
    return (unsigned int) (bc + 1);
}

unsigned long FourRussians::lookUpIndex(unsigned char xIndex, unsigned char yIndex,
        string const& x, string const& y,
        char *b, char *c) {
    unsigned long d = 1U;

    unsigned long result = 0U;

    char valT = 7, valG = 7, valC = 7, valA = 7;
    char current;
    unsigned char number = 0;

    for (auto i = yIndex - 1; i >= 0; i--) {

        switch (y[i]) {
            case 'A':
                if (valA == 7) {
                    valA = number;
                    number++;
                }
                result += valA * d;
                break;
            case 'T':
                if (valT == 7) {
                    valT = number;
                    number++;
                }
                result += valT * d;
                break;
            case 'G':
                if (valG == 7) {
                    valG = number;
                    number++;
                }
                result += valG * d;
                break;
            case 'C':
                if (valC == 7) {
                    valC = number;
                    number++;
                }
                result += valC * d;
                break;
        }
        d *= 4;
    }

    for (auto i = xIndex - 1; i >= 0; i--) {

        switch (x[i]) {
            case 'A':
                if (valA == 7) {
                    valA = number;
                    number++;
                }
                result += valA * d;
                break;
            case 'T':
                if (valT == 7) {
                    valT = number;
                    number++;
                }
                result += valT * d;
                break;
            case 'G':
                if (valG == 7) {
                    valG = number;
                    number++;
                }
                result += valG * d;
                break;
            case 'C':
                if (valC == 7) {
                    valC = number;
                    number++;
                }
                result += valC * d;
                break;
        }
        d *= 4;
    }

    for (unsigned int i = 0; i < (xIndex + yIndex); i++) result *= 3; // fast pow() function!

    d = 1U;

    for (auto i = yIndex - 1; i >= 0; i--) {
        result += bc_to_index(c[i]) * d;
        d *= 3;
    }

    for (auto i = xIndex - 1; i >= 0; i--) {
        result += bc_to_index(b[i]) * d;
        d *= 3;
    }
    return result;
}

//void FourRussians::print(Block &blk, unsigned char xLength, unsigned char yLength) {

//    cout << "TB" << "| x  ";
//
//    for (unsigned int col = 0; col < xLength; col++) {
//        cout << (int) blk.XY[col] << "  ";
//    }
//
//    cout << endl << "--+";
//    for (unsigned int col = 0; col <= xLength; col++) {
//        cout << "---";
//    }
//
//    for (unsigned int row = 0; row <= yLength; row++) {
//        if (row == 0) {
//            cout << endl << "x | ";
//        } else {
//            cout << endl << (int) blk.XY[xLength + row - 1] << " | ";
//        }
//        for (unsigned int col = 0; col <= xLength; col++) {
//            cout << (int) DATA(table, xLength + 1, row, col) << "  ";
//        }
//    }
//    cout << endl;
//}

