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

TableBlock::TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
                       string x, string y, unsigned long a,
                       unsigned char numCol){

    // TODO: bounds, ranges and sizes check...

    if(numCol == 0){
        numCol = t;
    }
    COL = numCol;
    T = t;
    A = a;

    // convert from offset to real
    B.reserve(T);
    char sum;
    sum = (char) (0 + A);
    //cout << "(0) " << b.size() << endl;
    for(unsigned char i = 0; i < COL; i++){
        //cout << "(1) " << B.size() << endl;
        B.push_back( (long) (sum + b.at(i)) );
        //cout << "(2) " << B.size() << endl;
        sum += b.at(i);
    }

    C.reserve(T);
    sum = (char) (0 + A);

    for(unsigned char i = 0; i < T; i++){
        C.push_back( (long) (sum + c.at(i)) );
        sum += c.at(i);
    }

    X = (x);
    Y = (y);
}

void TableBlock::calculate() {

    table.reserve((unsigned long) (T + 1) * (COL + 1));

    for(int i = 0; i < (T + 1) * (COL + 1); i++){
        table.push_back(0);
    }

    table.at(0) = A;

    // fill a first row
    for(unsigned char i = 1; i <= COL; i++){
        DATA(table, COL + 1, 0, i)  = (long) B.at((unsigned long) (i - 1));
    }

    // fill a first column
    for(unsigned char i = 1; i <= T; i++){
        DATA(table, COL + 1, i, 0)  = (long) C.at((unsigned long) (i - 1));
    }

    //this->print();

    for(unsigned char row = 1; row <= T; row++){
        for(unsigned char col = 1; col <= COL; col++){

            long top, left, diagonal, diagonalValue, cell;

            diagonal = DATA(table, COL + 1, row - 1, col - 1);
            top  = DATA(table, COL + 1, row - 1, col    );
            left = DATA(table, COL + 1, row, col - 1    );

            if(X[col - 1] == Y[row - 1]){
                diagonalValue = diagonal;
            }
            else{
                diagonalValue = diagonal + 1;
            }

            cell = min(min(diagonalValue, left + 1), top + 1);

            DATA(table, COL + 1, row, col) = cell;

        }
    }


    // last column calculation

    lastColumn.reserve(T);

    long first = DATA(table, COL, 0, COL);
    for(int i = 0; i < T; i++){
        long r = DATA(table, COL + 1, i + 1, COL);
        lastColumn.push_back((char) (r - first));
        first = r;
    }

    // last row calculation

    lastRow.reserve(COL);

    first = DATA(table, COL + 1, T, 0);
    for(int i = 0; i < COL; i++){
        long r = DATA(table, COL + 1, T, i + 1);
        lastRow.push_back((char) (r - first));
        first = r;
    }


    isCalculated = true;
}

void TableBlock::emptyTemp() {
    B = vector<long>();
    C = vector<long>();
    table = vector<long>();
}

void TableBlock::print() {
    if(!isCalculated){
        return;
    }

    cout << "TB" << "| x  ";

    for(unsigned int col = 0; col < COL; col++){
        cout << X[col] << "  ";
    }

    cout << endl << "−−+";
    for(unsigned int col = 0; col <= T; col++){
        cout << "−−−";
    }

    for(unsigned int row = 0; row <= T; row++){
        if(row == 0){
            cout << endl << "x | ";
        }
        else{
            cout << endl << Y[row - 1] << " | ";
        }
        for(unsigned int col = 0; col <= COL; col++) {
            cout << DATA(table, COL + 1, row, col) << "  ";
        }
    }
    cout << endl;
}

vector<char> TableBlock::verticalF(bool print){

    if(print) {
        for (vector<char>::const_iterator i = lastColumn.begin(); i != lastColumn.end(); ++i)
            cout << int(*i) << ' ';
        cout << endl;
    }

    return lastColumn;
}

vector<char> TableBlock::horizontalF(bool print){

    if(print) {
        for (vector<char>::const_iterator i = lastRow.begin(); i != lastRow.end(); ++i)
            std::cout << int(*i) << ' ';
        cout << endl;
    }

    return lastRow;
}