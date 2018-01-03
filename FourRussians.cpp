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

    // initialize array for storing calculated blocks
    genBlocks = new uint16_t[numComb];

    blocks = new uint32_t*[numRowsToCalculate];

    for (int row = 0; row < numRowsToCalculate; row++) {
        blocks[row] = new uint32_t[numBlocksPerRow];
    }

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

unsigned long FourRussians::calculateEditDistance() {

    uint8_t curC = firstRC;

    // first row calculation
    for (int col = 0; col < numBlocksPerRow; col++) {

        blocks[0][col] = mergeHash(xHash[col], yHash[0], firstRC, curC);
        //lastColumn
        curC = (genBlocks[blocks[0][col]] >> (T << 1)) & mask;
    }
    // rest of the matrix
    for (int row = 1; row < numRowsToCalculate; row++) {
        curC = firstRC;
        for (int col = 0; col < numBlocksPerRow; col++) {

            blocks[row][col] = mergeHash(
                    xHash[col],
                    yHash[row],
                    genBlocks[blocks[row - 1][col]] & mask, // lastRow -> b
                    curC); // lastColumn -> c
            //lastColumn
            curC = (genBlocks[blocks[row][col]] >> (T << 1)) & mask;
        }
    }

    // calculate the result by adding the lastRow of blocks in last row
    unsigned long result = yLen;

    for (int i = 0; i < numBlocksPerRow; i++) {
        for (uint8_t j = 0; j < T; j++) {
            result += (genBlocks[blocks[numRowsToCalculate - 1][i]] >> (j << 1) & 3) - 1;
        }
    }
    return result;
}

uint32_t FourRussians::mergeHash(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c) {

    return (
            (b << ((T << 2) + (T << 1))) // shift b to the begining of index
            + (c << (T << 2)) // shift c to be behind b
            + (yHash << (T << 1)) // shift xHash to be behind c
            + xHash // xHash is at the end of index
            );
}

void FourRussians::calculateEditScript() {

    int col = numBlocksPerRow - 1;
    int row = numRowsToCalculate - 1;

    string x = "";
    string oper = "";
    string y = "";

    uint8_t xHash, yHash, b, c;
    uint32_t index;

    uint8_t tableRow = 0;
    uint8_t tableCol = 0;

    int8_t diagonal, top, left;

    while (row > -1 && col > -1) {

        if (tableRow == 0) tableRow = T;
        if (tableCol == 0) tableCol = T;

        index = blocks[row][col];

        xHash = index & mask;
        yHash = (index >> (T << 1)) & mask;
        c = (index >> (T << 2)) & mask;
        b = (index >> ((T << 2) + (T << 1))) & mask;

        calculateBlock(1, 1, xHash, yHash, b, c);

        while (tableRow > 0 && tableCol > 0) {

            diagonal = table[tableRow - 1][tableCol - 1];
            left = table[tableRow][tableCol - 1];
            top = table[tableRow - 1][tableCol];

            if (diagonal <= left && diagonal <= top) {
                x += X[col * T + tableCol - 1];
                y += Y[row * T + tableRow - 1];

                if (X[col * T + tableCol - 1] == Y[row * T + tableRow - 1]) {
                    oper += "|";
                } else {
                    oper += ".";
                }

                tableRow--;
                tableCol--;

            } else if (left >= top) {
                x += '-';
                y += Y[row * T + tableRow - 1];
                oper += " ";
                tableRow--;
            } else {
                x += X[col * T + tableCol - 1];
                y += '-';
                oper += " ";
                tableCol--;
            }
        }

        if (tableRow == 0 && tableCol == 0) {
            row--;
            col--;
        } else if (tableRow != 0) {
            col--;
        } else {
            row--;
        }
    }

    while (row > -1) {

        for (int i = tableRow + 1; i < T; i++) {

            y += Y[row * T + i];
            x += '-';
            oper += " ";
        }
        tableRow = 0;
        row--;
    }

    while (col > -1) {

        for (int i = tableCol + 1; i < T; i++) {
            x += X[col * T + i];
            y += '-';
            oper += " ";
        }
        tableCol = 0;
        col--;
    }

    ofstream file("output.txt");

    for (int i = x.size() - 1; i >= 0; i--) {
        file << x[i];
    }
    file << endl;
    for (int i = oper.size() - 1; i >= 0; i--) {
        file << oper[i];
    }
    file << endl;
    for (int i = y.size() - 1; i >= 0; i--) {
        file << y[i];
    }
    file << endl;
    file.close();
}

uint16_t FourRussians::calculateBlock(uint8_t rowIndex, uint8_t colIndex, uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {
    
    // convert offset to real and fill the first row
    for (uint8_t i = colIndex; i <= T; i++) {
        // B row
        table[0][i] = (table[0][i - 1] + ((b >> ((T - i) << 1)) & 3)) - 1;
    }
    // convert offset to real and fill the first column
    for (uint8_t i = rowIndex; i <= T; i++) {
        // C column
        table[i][0] = (table[i - 1][0] + ((c >> ((T - i) << 1)) & 3)) - 1;
    }

    // calculate the block
    for (uint8_t row = rowIndex; row <= T; row++) {
        for (uint8_t col = colIndex; col <= T; col++) {

            if (((xHash >> ((T - col) << 1)) & 3)
                    != ((yHash >> ((T - row) << 1)) & 3)) {
                table[row][col] =
                        min(
                        min(table[row - 1][ col - 1],
                        table[row - 1][ col]
                        ), table[row][ col - 1]) + 1;
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
    
    //print (xHash, yHash );
    //cout << endl;
    return f;
}

uint16_t FourRussians::calculateBlockFast(uint8_t rowIndex, uint8_t colIndex, uint8_t xHash, uint8_t yHash,
        uint8_t b, uint8_t c) {

 //   cout << (int) rowIndex << " " << (int)colIndex << endl;
    
    // convert offset to real and fill the first row
    for (uint8_t i = colIndex; i <= T; i++) {
        // B row
        table[0][i] = (table[0][i - 1] + ((b >> ((T - i) << 1)) & 3)) - 1;
    }
    // convert offset to real and fill the first column
    for (uint8_t i = rowIndex; i <= T; i++) {
        // C column
        table[i][0] = (table[i - 1][0] + ((c >> ((T - i) << 1)) & 3)) - 1;
    }

    // calculate columns right
    for (uint8_t row = 1; row < rowIndex; row++) {
        for (uint8_t col = colIndex; col <= T; col++) {

            if (((xHash >> ((T - col) << 1)) & 3)
                    != ((yHash >> ((T - row) << 1)) & 3)) {
                table[row][col] =
                        min(
                        min(table[row - 1][ col - 1],
                        table[row - 1][ col]
                        ), table[row][ col - 1]) + 1;
            } else {
                table[row][col] = table[row - 1][ col - 1];
            }
        }
    }
    
    // calculate rows lower
    for (uint8_t row = rowIndex; row <= T; row++) {
        for (uint8_t col = 1; col <= T; col++) {

            if (((xHash >> ((T - col) << 1)) & 3)
                    != ((yHash >> ((T - row) << 1)) & 3)) {
                table[row][col] =
                        min(
                        min(table[row - 1][ col - 1],
                        table[row - 1][ col]
                        ), table[row][ col - 1]) + 1;
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
    
    //print (xHash, yHash );
    //cout << endl;
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

void FourRussians::print(uint8_t xHash, uint8_t yHash) {

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

void FourRussians::generateBlocks(uint8_t index, uint8_t rowIndex, uint8_t colIndex,
        uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c) {

    uint8_t m = (~0) << ((index + 1) << 1);

    for (uint8_t xValue = 0; xValue < 4; xValue++) {
        xHash = (xHash & m) + (xValue << (index << 1));
        for (uint8_t bValue = 0; bValue < 3; bValue++) {
            b = (b & m) + (bValue << (index << 1));
            for (uint8_t yValue = 0; yValue < 4; yValue++) {
                yHash = (yHash & m) + (yValue << (index << 1));
                for (uint8_t cValue = 0; cValue < 3; cValue++) {
                    c = (c & m) + (cValue << (index << 1));
                    if (index == 0) {
                        
                        genBlocks[mergeHash(xHash, yHash, b, c)] =
                        calculateBlockFast(rowIndex, colIndex, xHash, yHash, b, c);
                        colIndex = T + 1;
                        rowIndex = T;
                    } else {
                        generateBlocks(index - 1, rowIndex, colIndex, xHash, yHash, b, c);
                        colIndex = T;
                        rowIndex = T - index;
                    }
                }
            }
            colIndex = T - index;
            rowIndex = T - index;
        }
    }
}

