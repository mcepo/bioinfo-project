//
// Created by Marko Čepo and Tomislav Lokotar on 11/4/17.
//

#include "FourRussians.h"

// constructor
FourRussians::FourRussians(string &x, string &y, int blockSize) {


    xHash = yHash = b = c = 0;

    // make sure the longest string is X
    if (x.size() >= y.size()) {
        X = (x);
        Y = (y);
        switchedXY = false;
    } else {
        X = (y);
        Y = (x);
        switchedXY = true;
    }

    // store X&Y lengths
    xLen = X.size();
    yLen = Y.size();

    // calculate optimal T for string length
    optimalTcalc();

    // if block size isn't set, set T to optimal value for given strings
    if (blockSize == 0) {
        T = optimalT;
    } else {
        T = blockSize;
    }

    // appending char A to begining of string so that it is dividable by T
    expandXYforTsize();

    generateXYHashes();

    numComb = numCombinations();

    // initialize array for storing calculated blocks
    genBlocks = new uint16_t[numComb];

    matrix = new uint32_t*[numRowsToCalculate];

    // initialize 2d array that will be used in method
    // calculateBlocks for calculating the block
    // always the same
    table = new int8_t*[(T + 1)];
    for (uint8_t i = 0; i < (T + 1); i++) {
        table[i] = new int8_t[(T + 1)];
    }
    table[0][0] = 0;

    // assigning 10101010 value to firstRC
    // equal to having +1 in each field of b or c
    firstRC = 0;
    mask = 0;
    for (int8_t i = 0; i < T; i++) {
        firstRC = (firstRC << 2) + 2;
        mask = (mask << 2) + 3;
    }
}

// expand X and Y so they can fit all the blocks (i.e be divisible by)
void FourRussians::expandXYforTsize() {

    xMod = 0;
    yMod = 0;

    while ((xLen % T) != 0) {
        X += 'A';
        xLen++;
        xMod++;
    }
    while ((yLen % T) != 0) {
        Y += 'A';
        yLen++;
        yMod++;
    }

    numBlocksPerRow = xLen / T;
    numRowsToCalculate = yLen / T;
}

// calculate optimal T using formula
void FourRussians::optimalTcalc() {
    int len = max(xLen, yLen);
    double t = (log(len) / log(12)) / 2;
    optimalT = ((int) t + 1);

    if (optimalT < 2) {
        optimalT = 2;
    }
}

// X and Y hashes. List of them is to be generated, for faster use later on (while generatin hashes)
void FourRussians::generateXYHashes() {

    xHashs = new uint8_t[numBlocksPerRow];
    yHashs = new uint8_t[numRowsToCalculate];

    for (int i = 0; i < numBlocksPerRow; i++) {
        xHashs[i] = 0;
        for (uint8_t j = 0; j < T; j++) {
            xHashs[i] = (xHashs[i] << 2) + acgt_to_index(X[i * T + j]);
        }
    }

    for (int i = 0; i < numRowsToCalculate; i++) {
        yHashs[i] = 0;
        for (uint8_t j = 0; j < T; j++) {
            yHashs[i] = (yHashs[i] << 2) + acgt_to_index(Y[i * T + j]);
        }
    }
}

void FourRussians::calculateMatrix() {

    uint8_t curC = firstRC;

    int shift = numBlocksPerRow / numRowsToCalculate;
    double shiftReal = ((double) numBlocksPerRow / numRowsToCalculate);
    double shiftTotal = 0;

    rowOffset = new int[numRowsToCalculate];

    // TODO: define reduction factor by the number of blocks in row
    double reductionFactor;

    if (yLen >= 1000000) {
        reductionFactor = 0.01*((double)3/T);
    } else if (yLen >= 100000) {
        reductionFactor = 0.04*((double)3/T);
    } else if (yLen >= 10000) {
        reductionFactor = 0.08*((double)3/T);
    } else if (yLen >= 1000) {
        reductionFactor = 0.4*((double)3/T);
    } else {
        reductionFactor = 1;
    }

    int rowLng = ceil(numBlocksPerRow * reductionFactor) / 2;

    shiftTotal = -rowLng;
    int colStop = rowLng + shift ;

    int row = 1;
    int col;

    if (yLen <= 100) {
        colStop = numBlocksPerRow;
        shiftReal = 1;
    }

    matrix[0] = new uint32_t[colStop];
    rowOffset[0] = 0;

    // first row calculation
    for (int col = 0; col < colStop; col++) {

        matrix[0][col] = mergeHash(xHashs[col], yHashs[0], firstRC, curC);
        //lastColumn
        curC = (genBlocks[matrix[0][col]] >> (T << 1)) & mask;
    }

    // while the row starts with 0 do
    while (shiftTotal <= 0 && row < numRowsToCalculate) {

        curC = firstRC;

        rowOffset[row] = 0;

        matrix[row] = new uint32_t[colStop + shift ];

        for (col = 0; col < colStop; col++) {

            matrix[row][col] = mergeHash(
                    xHashs[col],
                    yHashs[row],
                    (genBlocks[matrix[row - 1][col]] & mask), // lastRow -> b
                    curC); // lastColumn -> c
            //lastColumn
            curC = (genBlocks[matrix[row][col]] >> (T << 1)) & mask;
        }
        colStop += shift;

        for (; col < colStop; col++) {
            matrix[row][col] = mergeHash(
                    xHashs[col],
                    yHashs[row],
                    firstRC, // lastRow -> b
                    curC); // lastColumn -> c

            //lastColumn
            curC = (genBlocks[matrix[row][col]] >> (T << 1)) & mask;
        }
        shiftTotal += shiftReal;
        row++;
    }

    shiftTotal = shiftReal;
    int diff;

    // when the col doesn't start with 0
    while ((colStop + shiftTotal) <= numBlocksPerRow) {

        curC = firstRC;
        rowOffset[row] = shiftTotal;
        diff = rowOffset[row] - rowOffset[row - 1];
        matrix[row] = new uint32_t[colStop];
        for (col = 0; col < colStop - diff; col++) {
            matrix[row][col] = mergeHash(
                    xHashs[col + rowOffset[row]],
                    yHashs[row],
                    (genBlocks[matrix[row - 1][col + diff]] & mask), // lastRow -> b
                    curC); // lastColumn -> c
            //lastColumn
            curC = (genBlocks[matrix[row][col]] >> (T << 1)) & mask;
        }

        for (; col < colStop; col++) {

            matrix[row][col] = mergeHash(
                    xHashs[col + rowOffset[row]],
                    yHashs[row],
                    firstRC, // lastRow -> b
                    curC); // lastColumn -> c

            //lastColumn
            curC = (genBlocks[matrix[row][col]] >> (T << 1)) & mask;
        }
        shiftTotal += shiftReal;
        row++;
    }

    // for the rest of matrix, it ends on numRowsToCalculate
    for (; row < numRowsToCalculate; row++) {

        curC = firstRC;
        rowOffset[row] = shiftTotal;
        diff = rowOffset[row] - rowOffset[row - 1];
        matrix[row] = new uint32_t[colStop];
        for (col = 0; col < colStop; col++) {
            matrix[row][col] = mergeHash(
                    xHashs[col + rowOffset[row]],
                    yHashs[row],
                    (genBlocks[matrix[row - 1][col + diff]] & mask), // lastRow -> b
                    curC); // lastColumn -> c
            //lastColumn
            curC = (genBlocks[matrix[row][col]] >> (T << 1)) & mask;
        }
        shiftTotal += shiftReal;
        colStop -= shift;
    }
}

// combine 'hashes' into a single 'hash' (index)
uint32_t FourRussians::mergeHash(uint8_t xHash, uint8_t yHash, uint8_t b, uint8_t c) {

    return (
            (b << ((T << 2) + (T << 1))) // shift b to the beginning of index
            + (c << (T << 2)) // shift c to be behind b
            + (yHash << (T << 1)) // shift xHash to be behind c
            + xHash // xHash is at the end of index
            );
}

uint32_t FourRussians::calculateEditDistanceAndScript(string outputPath) {


    // start at last row and column
    int col = numBlocksPerRow - 1;
    int row = numRowsToCalculate - 1;

    // initialize the result representing sequences
    string x = "";
    string oper = "";
    string y = "";

    // init. edit distance
    uint32_t result = 0;

    uint8_t tableRow = 0;
    uint8_t tableCol = 0;

    if (xMod != 0) {
        tableCol = T - xMod;
    }

    if (yMod != 0) {
        tableRow = T - yMod;
    }

    // relevant elements for construction of edit script
    // relevantni elementi na kojima se bazira odluka
    int8_t diagonal, top, left;

    // do while indexes of DP table are inside it (out -> done)
    // radim ovo dok jedan od indeksa ne izleti van DP tablice
    while (row > -1 && col > -1) {

        if (tableRow == 0) tableRow = T;
        if (tableCol == 0) tableCol = T;

        // use cached table block. This is the index of it
        index = matrix[row][col - rowOffset[row] ];

        // calculate block with given index
        xHash = index & mask;
        yHash = (index >> (T << 1)) & mask;
        c = (index >> (T << 2)) & mask;
        b = (index >> ((T << 2) + (T << 1))) & mask;
        
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

        // inside of block
        while (tableRow > 0 && tableCol > 0) {

            // relevant elements
            diagonal = table[tableRow - 1][tableCol - 1];
            left = table[tableRow][tableCol - 1];
            top = table[tableRow - 1][tableCol];
            
            // the old-fashioned (NW-way) of calculating edit script
            if (diagonal <= left && diagonal <= top) {
                x += X[col * T + tableCol - 1];
                y += Y[row * T + tableRow - 1];
                if (X[col * T + tableCol - 1] == Y[row * T + tableRow - 1]) {
                    oper += "|";
                } else {
                    oper += ".";
                    result++;
                }

                tableRow--;
                tableCol--;

            } else if (left <= top) {
                x += X[col * T + tableCol - 1];
                y += '-';
                oper += " ";
                tableCol--;
                result++;
            } else {

                x += '-';
                y += Y[row * T + tableRow - 1];
                oper += " ";
                result++;
                tableRow--;
            }
        }

        // calculates if recalculation of block (upper, left, or upper-left diagonal block) is needed
        if (tableRow == 0 && tableCol == 0) {
            row--;
            col--;

        } else if (tableRow != 0) {
            col--;
        } else {
            row--;
        }
    }

    // last-element edit script calculation: (for row and column)

    if (row > -1) {

        if (tableRow == 0) tableRow = T;

        for (int i = (row * T + tableRow - 1); i >= 0; i--) {

            y += Y[i];
            x += '-';
            oper += " ";
            result++;
        }
    }

    if (col > -1) {

        if (tableCol == 0) tableCol = T;

        for (int i = (col * T + tableCol - 1); i >= 0; i--) {

            x += X[i];
            y += '-';
            oper += " ";
            result++;
        }
    }

    outputMAF(x, y, result, outputPath);

//
//    ofstream file(outputPath);
//
//    for (int i = x.size() - 1; i >= 0; i--) {
//        file << x[i];
//    }
//    file << endl;
//    for (int i = oper.size() - 1; i >= 0; i--) {
//        file << oper[i];
//    }
//    file << endl;
//    for (int i = y.size() - 1; i >= 0; i--) {
//        file << y[i];
//    }
//    file.close();
//
    return result;
}

// MAF format output of sequence alignments
void FourRussians::outputMAF(string sequence1, string sequence2, unsigned long score, string outputPath){
    ofstream file(outputPath);

    file << "##maf version=1 scoring=Levenshtein program=bioinf-projekt-FourRussians-FER2017_2018-Cepo-Lokotar" << endl;
    file << endl;
    file << "a score=" << score << ".0" << endl << endl;

    if (!switchedXY) {
        file << "s sequence1 0 " << xLen - xMod << " " << "+" << " " << xLen - xMod << " " << sequence1 << endl;
        file << "s sequence2 0 " << yLen - yMod << " " << "+" << " " << yLen - yMod << " " << sequence2 << endl;
    }
    else{
        file << "s sequence1 0 " << yLen - yMod << " " << "+" << " " << yLen - yMod << " " << sequence2 << endl;
        file << "s sequence2 0 " << xLen - xMod << " " << "+" << " " << xLen - xMod << " " << sequence1 << endl;
    }
    file << endl;

    file.close();
};

uint8_t FourRussians::acgt_to_index(const char acgt) {
    if (acgt == 'A') return 0;
    if (acgt == 'C') return 1;
    if (acgt == 'G') return 2;
    if (acgt == 'T') return 3;
}

// calculates how many possible (different) blocks are there
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

// print table block...
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

// generate all blocks
void FourRussians::generateBlocks(uint8_t level, uint8_t rowIndex, uint8_t colIndex) {

    uint8_t m = (~0) << ((level + 1) << 1);
    for (uint8_t yValue = 0; yValue < 4; yValue++) {
        yHash = (yHash & m) + (yValue << (level << 1));
        for (uint8_t cValue = 0; cValue < 3; cValue++) {
            c = (c & m) + (cValue << (level << 1));
            for (uint8_t xValue = 0; xValue < 4; xValue++) {
                xHash = (xHash & m) + (xValue << (level << 1));
                for (uint8_t bValue = 0; bValue < 3; bValue++) {
                    b = (b & m) + (bValue << (level << 1));
                    if (level == 0) {
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
                        genBlocks[mergeHash(xHash, yHash, b, c)] = f;

                    } else {
                        generateBlocks(level - 1, rowIndex, colIndex);
                    }
                    colIndex = T - level;
                    rowIndex = T - level + 1;
                }
            }
            colIndex = T - level;
            rowIndex = T - level;
        }
    }
}
