//
// Created by loki on 11/4/17.
//

#include "FourRussians.h"


//FourRussians::FourRussians(unsigned char t, unsigned long a, vector<unsigned long> b, vector<unsigned long> c,
//                       vector<char> x, vector<char> y){
//
//    // deprecated
//
//    T = t;
//    A = a;
//    B = move(b);
//    C = move(c);
//    X = move(x);
//    Y = move(y);
//
//}

FourRussians::FourRussians() {
}

FourRussians::FourRussians(string x, string y, int blockSize) {

    T = blockSize;
    // TODO: bounds, ranges and sizes check...

    X = (x);
    Y = (y);

    found = 0;

    // appending "empty" characters to end of string so that blocks
    // can be fixed size
    int modX = X.size() % blockSize;
    if (modX != 0) {
        int appendLengthX = blockSize - modX;
        for (int i = 0; i < appendLengthX; i++) {
            X += '*';
        }
    }
    int modY = Y.size() % blockSize;
    if (modY != 0) {
        int appendLengthY = blockSize - modY;
        for (int i = 0; i < appendLengthY; i++) {
            Y += '*';
        }
    }

    xLen = X.size();
    yLen = Y.size();

    numBlocksPerRow = xLen / blockSize;
    numRowsToCalculate = yLen / blockSize;

    blocks.reserve((unsigned long) numBlocksPerRow);
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
    for (int col = 0; col < (numBlocksPerRow - 1); col++) {

        blocks.push_back(getTableBlock(currentB, currentC, &X[T * col], &Y[0]));
        currentC = &(blocks.at((unsigned long) col)->lastColumn);
    }

    // last column in first row
    blocks.push_back(getTableBlock(currentB, currentC, &X[T * (numBlocksPerRow - 1)], &Y[0]));

    // rest of the matrix

    for (int row = 1; row < (numRowsToCalculate - 1); row++) {

        calculateRow(row);
    }
    
    // last row calculation
    calculateRow((numRowsToCalculate - 1));

    unsigned long result = 0L;

    for (auto &block : blocks) {
        for (unsigned long j = 0; j < block->lastRow.size(); j++) {
            result += block->lastRow.at(j);
        }
    }

    result += yLen;
    return result;
}

void FourRussians::calculateRow(int index) {

    vector<char> *currentC, *currentB;

    currentC = &constBC;

    for (int col = 0; col < (numBlocksPerRow - 1); col++) {

        currentB = &(blocks.at((unsigned int) col)->lastRow);
        blocks.at((unsigned int) col) = getTableBlock(currentB, currentC, &X[T * col], &Y[T * index]);
        currentC = &(blocks.at((unsigned long) col)->lastColumn);
    }

    // last column in row
    currentB = &(blocks.at((unsigned int) (numBlocksPerRow - 1))->lastRow);
    blocks.at((unsigned int) (numBlocksPerRow - 1)) = getTableBlock(currentB, currentC, &X[T * (numBlocksPerRow - 1)], &Y[T * index]);
}

Block* FourRussians::getTableBlock(vector<char> *b, vector<char> *c,
        string const& x, string const& y) {

    Block blk = {*b, *c, getXY(x, y)};

    auto foundBlock = generatedBlocks.find(blk);

    if (foundBlock == generatedBlocks.end()) {
        calculateBlock(blk);
        generatedBlocks.insert({blk, blk});
        return &(generatedBlocks.find(blk)->second);
    } else {
        found++;
        return &(foundBlock->second);
    }
}

vector<char> FourRussians::getXY(string const& x, string const& y) {

    char valT = 0, valG = 0, valC = 0, valA = 0, valEmpty = 0;

    unsigned char number = 1;

    vector<char> XY;
    XY.reserve(T * 2);

    for (string::size_type i = 0; i < T && i < x.size(); ++i) {

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

    for (string::size_type i = 0; i < T && i < y.size(); ++i) {

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

void FourRussians::calculateBlock(Block &blk) {

    char sumB = (char) 0;
    char sumC = (char) 0;

    // convert offset to real and fill the first column and row
    for (unsigned char i = 1; i <= T; i++) {
        sumB += blk.B.at((unsigned long) (i - 1));
        DATA(table, T + 1, 0, i) = (char) sumB;

        sumC += blk.C.at((unsigned long) (i - 1));
        DATA(table, T + 1, i, 0) = (char) sumC;
    }

    // TODO: probati implementirati da petlja ide samo do predzadnjeg redka i
    //      i predzadnjeg stupca, zatim zadnji stupac i redak posebno izračunati
    //      i odmah ga ubaciti u lastRow i lastColumn bloka
    for (unsigned char row = 1; row <= T; row++) {
        for (unsigned char col = 1; col <= T; col++) {

            diagonal = DATA(table, T + 1, row - 1, col - 1);
            top = DATA(table, T + 1, row - 1, col) + 1;
            left = DATA(table, T + 1, row, col - 1) + 1;

            if (blk.XY[col - 1] != blk.XY[T + row - 1]) {
                diagonal += 1;
            }
            DATA(table, T + 1, row, col) = min(min(diagonal, left), top);
        }
    }

    // last column calculation

    blk.lastColumn.reserve(T);

    long first = DATA(table, T, 0, T);
    long r;
    for (int i = 0; i < T; i++) {
        r = DATA(table, T + 1, i + 1, T);
        blk.lastColumn.push_back((char) (r - first));
        first = r;
    }

    // last row calculation

    blk.lastRow.reserve(T);

    first = DATA(table, T + 1, T, 0);
    for (int i = 0; i < T; i++) {
        r = DATA(table, T + 1, T, i + 1);
        blk.lastRow.push_back((char) (r - first));
        first = r;
    }
}

// TODO: ova metoda se trenutno ne koristi, možda implementirati da radi 
// za predani block
// 

void FourRussians::print() {

    cout << "TB" << "| x  ";

    for (unsigned int col = 0; col < T; col++) {
        cout << X[col] << "  ";
    }

    cout << endl << "--+";
    for (unsigned int col = 0; col <= T; col++) {
        cout << "---";
    }

    for (unsigned int row = 0; row <= T; row++) {
        if (row == 0) {
            cout << endl << "x | ";
        } else {
            cout << endl << Y[row - 1] << " | ";
        }
        for (unsigned int col = 0; col <= T; col++) {
            cout << DATA(table, T + 1, row, col) << "  ";
        }
    }
    cout << endl;
}