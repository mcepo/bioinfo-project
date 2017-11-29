//
// Created by loki on 11/4/17.
//

#include "TableBlock.h"


//TableBlock::TableBlock(unsigned char t, unsigned long a, vector<unsigned long> b, vector<unsigned long> c,
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

TableBlock::TableBlock() {}

TableBlock::TableBlock(string x, string y, int blockSize) {

    T = blockSize;
    // TODO: bounds, ranges and sizes check...
    
    X = (x);
    Y = (y);
    
    table.reserve((unsigned long) (T + 1) * (T + 1));

    for (int i = 0; i < (T + 1) * (T + 1); i++) {
        table.push_back(0);
    }
}

vector<char> TableBlock::getXY(string x, string y) {

    unordered_map<char, char> mapLetters;
    unsigned char number = 1;

    vector<char> XY;
    XY.reserve(T*2);

    for (string::size_type i = 0; i < T && i < x.size(); ++i) {

        auto foundLetter = mapLetters.find(x[i]);

        if (foundLetter == mapLetters.end()) {
            // nisam ga pronašao
            mapLetters.insert({x[i], number});
            XY.push_back(number);
            number++;

        } else {
            // pronašao sam ga
            XY.push_back(foundLetter->second);
        }
    }

    for (string::size_type i = 0; i < T && i < y.size(); ++i) {

        auto foundLetter = mapLetters.find(y[i]);

        if (foundLetter == mapLetters.end()) {
            // nisam ga pronašao
            mapLetters.insert({y[i], number});
            XY.push_back(number);
            number++;

        } else {
            // pronašao sam ga
            XY.push_back(foundLetter->second);
        }
    }
    return XY;
}

void TableBlock::calculateBlock(Block &blk) {
    
    char sumB = (char) 0;
    char sumC = (char) 0;
    
    // convert offset to real and fill the first column and row
    for (unsigned char i = 1; i <= T; i++) {
        sumB+=blk.B.at((unsigned long) (i - 1));
        DATA(table, T + 1, 0, i) = (char) sumB;
        
        sumC+=blk.C.at((unsigned long) (i - 1));
        DATA(table, T + 1, i, 0) = (char) sumC;
    }

    long top, left, diagonal, diagonalValue, cell;

    for (unsigned char row = 1; row <= T; row++) {
        for (unsigned char col = 1; col <= T; col++) {

            diagonal = DATA(table, T + 1, row - 1, col - 1);
            top = DATA(table, T + 1, row - 1, col);
            left = DATA(table, T + 1, row, col - 1);

            if (blk.XY[col - 1] == blk.XY[T + row - 1]) {
                diagonalValue = diagonal;
            } else {
                diagonalValue = diagonal + 1;
            }
            cell = min(min(diagonalValue, left + 1), top + 1);
            DATA(table, T + 1, row, col) = cell;
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

void TableBlock::print() {

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