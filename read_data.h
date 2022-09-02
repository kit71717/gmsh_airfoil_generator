//
// Created by Calvin Li on 2022-08-22.
//

#ifndef GMSH_AIRFOIL_GENERATOR_FILEREADER_H
#define GMSH_AIRFOIL_GENERATOR_FILEREADER_H

#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <numeric>
#include<iostream>
#include<iomanip>

using namespace std;

int fileReader(const char *s, vector< pair <double, double> >& airfoil_data){
    ifstream f(s);                                      //ifstream object on Input File
    string line;                                        //String for accessing each line being read

    int k = 0;
    double xPoint;
    double yPoint;
    double dataPoints;


    //READ the surface top and bottom then sort them and add them to the vector airfoil.

    //Read Input File and Store as Vector Elements
    while (getline(f, line)) {

        istringstream my_stream(line);

        while (my_stream >> dataPoints) {
            if (k == 0) {
                xPoint = dataPoints;
                k = 1;
            } else {
                yPoint = dataPoints;
            }
        }

        airfoil_data.push_back(make_pair(xPoint, yPoint));
        k = 0;

    }

    return airfoil_data.size();

}

/**
 * FOR PRINTING VECTOR PAIR
 * @param airfoil_data
 */
void printVectorPair(vector< pair <double, double> >& airfoil_data){

    cout << "PRINTING VECTOR PAIR" << endl;
    for (int i=0; i<airfoil_data.size(); i++) {
        cout << airfoil_data[i].first << " "
             << airfoil_data[i].second << endl;
    }
}

/**
 * Assertion Macro
 */
#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

/**
 * Assertion Macro
 * @param expr_str
 * @param expr
 * @param file
 * @param line
 * @param msg
 */
void __M_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
                  << "Expected:\t" << expr_str << "\n"
                  << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

#endif //GMSH_AIRFOIL_GENERATOR_FILEREADER_H
