/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Admin
 *
 * Created on September 5, 2017, 4:20 PM
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

void prettyPrint(int** D, string X, string Y) {
    
    
    ofstream outputFile("output.txt");

    if (!outputFile) {
        cout << "Unable to open output file";
    }
    
    outputFile << "\t\t*";
    for (int i = 0; i < X.size(); i++) {
        outputFile << "\t" << X[i];
    }
    outputFile << endl;

    outputFile << "\t*";

    for (int j = 0; j < Y.size()+1; j++) {
        for (int i = 0; i < X.size()+1; i++) {
            outputFile << "\t" << D[i][j];
        }
        outputFile << endl;
        outputFile << "\t" << Y[j];
    }
}

int main(int argc, char** argv) {

    int i;
    string X, Y;

    int** D = NULL;


    ifstream inputFile("input10.txt");
    if (inputFile) {
        getline(inputFile, X);
        getline(inputFile, Y);
    } else {
        cout << "Unable to open file";
        return -1;
    }

    cout << "X -> size: " << X.size() << " value: " << X << endl;
    cout << "Y -> size: " << Y.size() << " value: " << Y << endl;

    D = new int*[X.size() + 1];

    for (int i = 0, count = 0; i < X.size() + 1; i++) {
        D[i] = new int[Y.size() + 1 ];
        D[i][0] = count;
        D[0][i] = count;
        count++;
    }

    for (int j = 1; j < Y.size() + 1; j++) {
        for (int i = 1; i < X.size() + 1; i++) {
            if (X[i - 1] == Y[j - 1]) {
                D[i][j] = D[i - 1][j - 1];
            } else {

                D[i][j] = fmin(D[i - 1][j], D[i - 1][j - 1]);
                D[i][j] = fmin(D[i][j], D[i][j - 1]) + 1;
            }
        }
    }
    
    prettyPrint(D, X, Y);

    return 0;
}