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

TableBlock::TableBlock(vector<char> &b, vector<char> &c,
        string x, string y) {

    T = BLOCK_SIZE;

    unordered_map<char, char> mapLetters;
    unsigned char number = 1;

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
	
    // TODO: bounds, ranges and sizes check...

    // convert from offset to real
    B.reserve(T);
    char sum = (char) 0;

    for (unsigned char i = 0; i < T; i++) {
        sum += b.at(i);
        B.push_back((char) (sum));
    }

    C.reserve(T);
    sum = (char) 0;

    for (unsigned char i = 0; i < T; i++) {
        sum += c.at(i);
        C.push_back((char) (sum));
    }

    X = (x);
    Y = (y);
}

void TableBlock::calculate() {

    table.reserve((unsigned long) (T + 1) * (T + 1));

    for (int i = 0; i < (T + 1) * (T + 1); i++) {
        table.push_back(0);
    }

    // fill a first column and row
    for (unsigned char i = 1; i <= T; i++) {
        DATA(table, T + 1, 0, i) = (char) B.at((unsigned long) (i - 1));
        DATA(table, T + 1, i, 0) = (char) C.at((unsigned long) (i - 1));
    }

    long top, left, diagonal, diagonalValue, cell;

    for (unsigned char row = 1; row <= T; row++) {
        for (unsigned char col = 1; col <= T; col++) {

            diagonal = DATA(table, T + 1, row - 1, col - 1);
            top = DATA(table, T + 1, row - 1, col);
            left = DATA(table, T + 1, row, col - 1);

            if (X[col - 1] == Y[row - 1]) {
                diagonalValue = diagonal;
            } else {
                diagonalValue = diagonal + 1;
            }
            cell = min(min(diagonalValue, left + 1), top + 1);
            DATA(table, T + 1, row, col) = cell;
        }
    }

    // last column calculation

    lastColumn.reserve(T);

    long first = DATA(table, T, 0, T);
    for (int i = 0; i < T; i++) {
        long r = DATA(table, T + 1, i + 1, T);
        lastColumn.push_back((char) (r - first));
        first = r;
    }

    // last row calculation

    lastRow.reserve(T);

    first = DATA(table, T + 1, T, 0);
    for (int i = 0; i < T; i++) {
        long r = DATA(table, T + 1, T, i + 1);
        lastRow.push_back((char) (r - first));
        first = r;
    }

    isCalculated = true;
}

void TableBlock::emptyTemp() {
    B = vector<char>();
    C = vector<char>();
    table = vector<char>();
}

void TableBlock::print() {
    if (!isCalculated) {
        return;
    }

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

vector<char> TableBlock::verticalF(bool print) {

    if (print) {
        for (vector<char>::const_iterator i = lastColumn.begin(); i != lastColumn.end(); ++i)
            cout << int(*i) << ' ';
        cout << endl;
    }

    return lastColumn;
}

vector<char> TableBlock::horizontalF(bool print) {

    if (print) {
        for (vector<char>::const_iterator i = lastRow.begin(); i != lastRow.end(); ++i)
            std::cout << int(*i) << ' ';
        cout << endl;
    }

    return lastRow;
}