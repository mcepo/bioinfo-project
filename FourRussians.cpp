//
// Created by loki on 11/4/17.
//

#include "FourRussians.h"

FourRussians::FourRussians() {
}

FourRussians::FourRussians(string const &x, string const &y, int blockSize) {

    T = blockSize;

    xLen = x.size();
    yLen = y.size();

    if (xLen >= yLen) {
        X = (x);
        Y = (y);
        numBlocksPerRow = xLen / blockSize;
        numRowsToCalculate = yLen / blockSize;
    } else {
        X = (y);
        Y = (x);
        numBlocksPerRow = yLen / blockSize;
        numRowsToCalculate = xLen / blockSize;
    }

    cout << "Generating xy hashes... ";
    generateXYHashes();
    cout << "GENERATED" << endl;

    numComb = numCombinations();

    // initialize array for last row
    blocks = new uint16_t[numBlocksPerRow];
    // initialize array for storing calculated blocks
    genBlocks = new uint16_t[numComb];

    // initialize 2d array that will be used in method
    // calculateBlocks for calculating the block
    // always the same 
    table = new int8_t*[(T + 1)];
    for (uint8_t i = 0; i < (T + 1); i++) {
        table[i] = new int8_t[(T + 1)];
    }
    table[0][0] = 0;

    // assigning 10101010 value to firstRC
    // što je jednako kao da u svako polje stavim +1
    // stavljam u biti 2 u svako polje jer offsete spremam uvečano za 1
    // znači 0, 1 ili 2 umjesto -1 0 +1
    firstRC = 0;
    mask = 0;
    for (int8_t i = 0; i < T; i++) {
        firstRC = (firstRC << 2) + 2;
        mask = (mask << 2) + 3;
    }
}

void FourRussians::generateXYHashes() {

    xHash = new uint8_t[numBlocksPerRow];
    yHash = new uint8_t[numRowsToCalculate];

    for (int i = 0; i < numBlocksPerRow; i++) {
        xHash[i] = 0;
        for (uint8_t j = 0; j < T; j++) {
            xHash[i] = (xHash[i] << 2) + acgt_to_index(X[i * T + j]);
        }
    }

    for (int i = 0; i < numRowsToCalculate; i++) {
        yHash[i] = 0;
        for (uint8_t j = 0; j < T; j++) {
            yHash[i] = (yHash[i] << 2) + acgt_to_index(Y[i * T + j]);
        }
    }
}

unsigned long FourRussians::calculate() {

    uint8_t curC = firstRC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {

        blocks[col] = getTableBlock(xHash[col], yHash[0], firstRC, curC);
        //lastColumn
        curC = (blocks[col] >> (T << 1)) & mask;
    }
    // rest of the matrix
    for (int row = 1; row < numRowsToCalculate; row++) {
        calculateRow(row);
    }

    // calculate the result by adding the lastRow of blocks in last row
    unsigned long result = yLen;

    for (int i = 0; i < numBlocksPerRow; i++) {
        for (uint8_t j = 0; j < T; j++) {
            result += (blocks[i] >> (j << 1) & 3) - 1;
        }
    }
    return result;
}

void FourRussians::calculateRow(int row) {

    uint8_t curC = firstRC, curB;

    for (int col = 0; col < numBlocksPerRow; col++) {

        //lastRow
        curB = blocks[col] & mask;
        blocks[col] = getTableBlock(xHash[col], yHash[row], curB, curC);
        //lastColumn
        curC = (blocks[col] >> (T << 1)) & mask;
    }
}

uint16_t FourRussians::getTableBlock(uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

    // get index of searched block
    uint32_t index = mergeHash(xHash, yHash, b, c);

    // if block doesn't exist 
    if (genBlocks[index] == 0) {
        // calculate it
        genBlocks[index] = calculateBlock(xHash, yHash, b, c);
    }
    // return the searched block 
    return genBlocks[index];
}

uint32_t FourRussians::mergeHash(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c) {

    return ((b << ((T << 2) + (T << 1))) + (c << (T << 2)) + (yHash << (T << 1)) + xHash);
}

uint16_t FourRussians::calculateBlock(uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

    // convert offset to real and fill the first row and column
    for (uint8_t i = 1; i <= T; i++) {
        // B row
        table[0][i] = (table[0][i - 1] + ((b >> ((T - i) << 1)) & 3)) - 1;
        // C column
        table[i][0] = (table[i - 1][0] + ((c >> ((T - i) << 1)) & 3)) - 1;
    }

    // calculate the block
    for (uint8_t row = 1; row <= T; row++) {
        for (uint8_t col = 1; col <= T; col++) {

            if (((xHash >> ((T - col) << 1)) & 3) != ((yHash >> ((T - row) << 1)) & 3)) {
                diagonal = table[row - 1][ col - 1];
                top = table[row - 1][ col];
                left = table[row][ col - 1];
                table[row][col] = min(min(diagonal, left), top) + 1;
            } else {
                table[row][col] = table[row - 1][ col - 1];
            }
        }
    }
    //      c + b
    // lastColumn + lastRow
    uint16_t f = 0;

    // dodao sam +1 u offsete kako bi rješio problem sa -1 
    // i prebacivanjem istoga

    // last column
    for (uint8_t i = 0; i < T; i++) {
        f = (f << 2) + ((table[i + 1][T] - table[i][T]) + 1);
    }

    // last row
    for (uint8_t i = 0; i < T; i++) {
        f = (f << 2) + ((table[T][i + 1] - table[T][i]) + 1);
    }

    //  print (xHash, yHash, b ,c );

    return f;
}

uint8_t FourRussians::acgt_to_index(const char acgt) {
    if (acgt == 'A') return 0;
    if (acgt == 'C') return 1;
    if (acgt == 'G') return 2;
    if (acgt == 'T') return 3;
}

uint64_t FourRussians::numCombinations() {

    uint64_t result = 0U;

    for (int i = 0; i < (2 * T); i++) {
        result = (result << 2) + 2;
    }
    for (int i = 0; i < (2 * T); i++) {
        result = (result << 2) + 3;
    }

    return result;
}

void FourRussians::print(uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

    cout << "TB" << "| x  ";

    // print x
    for (unsigned int col = 0; col < T; col++) {
        cout << ((xHash >> (2 * (T - col - 1))) & 3) << "  ";
    }

    cout << endl << "--+";
    for (unsigned int col = 0; col <= T; col++) {
        cout << "---";
    }

    for (unsigned int row = 0; row <= T; row++) {
        if (row == 0) {
            cout << endl << "x | ";
        } else {
            cout << endl << ((yHash >> (2 * (T - row))) & 3) << " | ";
        }
        for (unsigned int col = 0; col <= T; col++) {
            cout << (int) table[row][col] << "  ";
        }
    }
    cout << endl;
}

