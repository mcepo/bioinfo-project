
/* 
 * File:   generator.cpp
 * Author: marko.cepo
 *
 * Created on November 5, 2017, 4:20 PM
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>

using namespace std;

#define MAX_STRING_LENGTH 100000
#define MIN_STRING_LENGTH 100

const char charMapper[] = "ACGT"; 

/*
 *
 */

int main(int argc, char** argv) {

    clock_t start = clock();
    int xLen;
    int yLen;
    string fileName = "input.txt";
    
    srand(time(0));

    if ( argc == 1){
        xLen = (rand() % MAX_STRING_LENGTH) + MIN_STRING_LENGTH;
        yLen = (rand() % MAX_STRING_LENGTH) + MIN_STRING_LENGTH;
    }else if ( argc == 2 ) {
        fileName = argv[1];
        xLen = (rand() % MAX_STRING_LENGTH) + MIN_STRING_LENGTH;
        yLen = (rand() % MAX_STRING_LENGTH) + MIN_STRING_LENGTH;
    } else if (argc == 3){
        fileName = argv[1];
        xLen = atoi(argv[2]);
        yLen = atoi(argv[2]);
    } else if (argc == 4) {
        fileName = argv[1];
        xLen = atoi(argv[2]);
        yLen = atoi(argv[3]);
    } else{
        cout << "Wrong number of arguments! \n\nUsage: generator outputFile firstSequenceLength [secondSequenceLength]" << endl;
    }

    cout << "Generating X length "<< xLen << " characters, Y length "<< yLen << "... ";

    string X="", Y="";

    ofstream outputFile(fileName);
    if (!outputFile) {
        cout << "Unable to open file";
        return -1;
    }

    srand(time(0));
    
    for (int i = 0; i < xLen; i++) {
        X = X + charMapper[(rand() % 4)] ;
    }
    
    for (int i = 0; i < yLen; i++) {
        Y = Y + charMapper[(rand() % 4)] ;
    }

    outputFile << X << "\n";
    outputFile << Y;

    outputFile.close();

    double execTime = (clock() - start) /(double) CLOCKS_PER_SEC;

    cout << "generated in " << execTime << "s." << endl;

return 0;
}
