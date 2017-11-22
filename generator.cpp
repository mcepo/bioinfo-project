
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


#define DEFAULT_STRING_LENGTH 100

const char charMapper[] = "ACGT"; 

/*
 *
 */

int main(int argc, char** argv) {

    clock_t start = clock();
    int inputStringLength;

    if ( argc != 2 ) {
        inputStringLength = DEFAULT_STRING_LENGTH;
    } else {
        inputStringLength = atoi(argv[1]);
    }

    cout << "Generating X&Y length "<< inputStringLength << " characters... ";


    string X="", Y="";

    ofstream outputFile("input.txt");
    if (!outputFile) {
        cout << "Unable to open file";
        return -1;
    }

    srand(time(0));

    for (int i = 0; i < inputStringLength; i++) {
        X = X + charMapper[(rand() % 4)] ;
        Y = Y + charMapper[(rand() % 4)] ;
    }

    outputFile << X << "\n";
    outputFile << Y;

    outputFile.close();

    double execTime = (clock() - start) /(double) CLOCKS_PER_SEC;

    cout << "generated in " << execTime << "s." << endl;

return 0;
}
