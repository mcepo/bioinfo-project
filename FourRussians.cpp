//
// Created by loki on 11/4/17.
//

#include "FourRussians.h"

FourRussians::FourRussians() {
}

FourRussians::FourRussians(string x, string y, int blockSize) {

    T = blockSize;
    Tp = T + 1;
    // TODO: bounds, ranges and sizes check...

    X = (x);
    Y = (y);

    found = 0;
    calculated = 0;

    xLen = X.size();
    yLen = Y.size();

    numBlocksPerRow = xLen / blockSize;
    numRowsToCalculate = yLen / blockSize;

    cout << "Generating xy hashes ... ";
    generateXYHashes();
    cout << "GENERATED" << endl;

    unsigned long numComb = numCombinations(T);

    blocks = new uint16_t[numBlocksPerRow];
    genBlocks = new uint16_t[numComb];

    cout << "Max number of blocks: " << numComb << endl;

    double mem = (numComb * (2 * T)) / 1024 / 1024;

    cout << "Expected memory usage: " << mem << "MB" << endl;

    table = new int8_t*[(T + 1)];
    for (int i = 0; i < (T + 1); i++) {
        table[i] = new int8_t[(T + 1)];
    }
    table[0][0] = 0;

    // assigning 10101010 value to firstRC
    // što je jednako kao da u svako polje stavim +1
    // stavljam u biti 2 u svako polje jer offsete spremam uvečano za 1
    // znači 0, 1 ili 2 umjesto -1 0 +1
    firstRC = 170;

}

void FourRussians::generateXYHashes() {

    xHash = new uint8_t*[numRowsToCalculate];
    yHash = new uint8_t[numRowsToCalculate];

    uint8_t valT, valG, valC, valA;
    uint8_t number;

    for (int row = 0; row < numRowsToCalculate; row++) {

        valT = 7, valG = 7, valC = 7, valA = 7;
        number = 0;

        for (int j = 0; j < T; j++) {

            switch (Y[row * T + j]) {
                case 'A':
                    if (valA == 7) {
                        valA = number;
                        number++;
                    }
                    yHash[row] = (yHash[row] << 2) + valA;
                    break;
                case 'T':
                    if (valT == 7) {
                        valT = number;
                        number++;
                    }
                    yHash[row] = (yHash[row] << 2) + valT;
                    break;
                case 'G':
                    if (valG == 7) {
                        valG = number;
                        number++;
                    }
                    yHash[row] = (yHash[row] << 2) + valG;
                    break;
                case 'C':
                    if (valC == 7) {
                        valC = number;
                        number++;
                    }
                    yHash[row] = (yHash[row] << 2) + valC;
                    break;
            }
        }

        xHash[row] = new uint8_t[numBlocksPerRow];
        for (int col = 0; col < numBlocksPerRow; col++) {
            for (int j = 0; j < T; j++) {

                switch (X[col * T + j]) {
                    case 'A':
                        if (valA == 7) {
                            valA = number;
                            number++;
                        }
                        xHash[row][col] = (xHash[row][col] << 2) + valA;
                        break;
                    case 'T':
                        if (valT == 7) {
                            valT = number;
                            number++;
                        }
                        xHash[row][col] = (xHash[row][col] << 2) + valT;
                        break;
                    case 'G':
                        if (valG == 7) {
                            valG = number;
                            number++;
                        }
                        xHash[row][col] = (xHash[row][col] << 2) + valG;
                        break;
                    case 'C':
                        if (valC == 7) {
                            valC = number;
                            number++;
                        }
                        xHash[row][col] = (xHash[row][col] << 2) + valC;
                        break;
                }
            }
        }
    }
}

//void FourRussians::generateXYHashes() {
//
//    xHash = new uint8_t[numBlocksPerRow];
//    yHash = new uint8_t[numRowsToCalculate];
//
//    for (int i = 0; i < numBlocksPerRow; i++) {
//        for (int j = 0; j < T; j++) {
//            xHash[i] = (xHash[i] << 2) + acgt_to_index(X[i * T + j]);
//        }
//    }
//
//    for (int i = 0; i < numRowsToCalculate; i++) {
//        for (int j = 0; j < T; j++) {
//            yHash[i] = (yHash[i] << 2) + acgt_to_index(Y[i * T + j]);
//        }
//    }
//}

unsigned long FourRussians::calculate() {

    uint8_t curC = firstRC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {

        blocks[col] = getTableBlock(xHash[0][col], yHash[0], firstRC, curC);
        //lastColumn
        curC = (blocks[col] >> 8);
    }
    // rest of the matrix
    for (int row = 1; row < numRowsToCalculate; row++) {
        calculateRow(row);
    }

    unsigned long result = yLen;

    for (int i = 0; i < numBlocksPerRow; i++) {
        for (unsigned long j = 0; j < T; j++) {
            result += (blocks[i] >> (2 * j) & 3) - 1;
        }
    }
    return result;
}

void FourRussians::calculateRow(int row) {

    uint8_t curC = firstRC, curB;

    for (int col = 0; col < numBlocksPerRow; col++) {

        //lastRow
        curB = blocks[col];
        blocks[col] = getTableBlock(xHash[row][col], yHash[row], curB, curC);
        //lastColumn
        curC = (blocks[col] >> 8);
    }
}

uint16_t FourRussians::getTableBlock(uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

    uint32_t index = mergeHash(xHash, yHash, b, c);

    if (genBlocks[index] == 0) {
        genBlocks[index] = calculateBlock(xHash, yHash, b, c);
        calculated++;
    } else {
        found++;
    }
    return genBlocks[index];
}

uint32_t FourRussians::mergeHash(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c) {

    return ((yHash << 24) + (xHash << 16) + (b << 8) + c);
}

uint16_t FourRussians::calculateBlock(uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

    // convert offset to real and fill the first row and column
    for (unsigned char i = 1; i <= T; i++) {
        // B row
        table[0][i] = (table[0][i-1] + ((b >> (2 * (T - i))) & 3)) - 1;
        // C column
        table[i][0] = (table[i-1][0] + ((c >> (2 * (T - i))) & 3)) - 1;
    }

    for (unsigned char row = 1; row <= T; row++) {
        for (unsigned char col = 1; col <= T; col++) {

            //ovdje radim pretpostavku da ukoliko su isti znakovi mogu prepisati sa
            // dijagonale, koliko sam testirao mislim da to prolazi
            if (((xHash >> (2 * (T - col))) & 3) != ((yHash >> (2 * (T - row))) & 3)) {
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
    for (int i = 0; i < T; i++) {
        f = (f << 2) + ((table[i+1][T] - table[i][T]) + 1);
    }

    // last row
    for (int i = 0; i < T; i++) {
        f = (f << 2) + ((table[T][i+1] - table[T][i]) + 1);
    }

    //  print (xHash, yHash, b ,c );

    return f;
}

//unsigned int FourRussians::acgt_to_index(const char acgt) {
//    if (acgt == 'A') return 0;
//    if (acgt == 'C') return 1;
//    if (acgt == 'G') return 2;
//    if (acgt == 'T') return 3;
//}


//TODO: ovdje nekako pametnije napraviti da se izračuna max broj
// blokova

unsigned long FourRussians::numCombinations(unsigned char T) {
    return (unsigned long) (3 * 3 * pow(4, 5) * pow(4, 8));
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
    cout << endl;
}

