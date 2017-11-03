
/* 
 * File:   generator.cpp
 * Author: marko.cepo
 *
 * Created on November 5, 2017, 4:20 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


using namespace std;

/*
 * 
 */

//    ofstream outputFile("output.txt");
//
//    if (!outputFile) {
//        cout << "Unable to open output file";
//    }


int main(int argc, char** argv) {

    char charMapper[] = "ACGT";

    int inputStringLength = 100;
    string X="", Y="";

    ofstream outputFile("input.txt");
    if (!outputFile) {
        cout << "Unable to open file";
        return -1;
    }


    for (int i = 0; i < inputStringLength; i++) {
        X = X + charMapper[(rand() % 4)] ;
        Y = Y + charMapper[(rand() % 4)] ;
    }

    cout << X << endl;
    outputFile << X << endl;
    cout << Y << endl;
    outputFile << Y;


return 0;
}
