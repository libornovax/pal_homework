//
// Libor Novak
// 21.12.2015
//
// PAL Homework 6 - Text Search
//

#include <iostream>
#include <cstdio>
#include <vector>

#include "PatternFinder.h"


void loadSettings (int &N, int &M, int &cI, int &cD, int &cR)
{
    // Read the settings
    std::cin >> N >> M >> cI >> cD >> cR;
}


int main (int argc, char* argv[])
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    // -- LOAD SETTINGS -- //
    int N; // Length of text T
    int M; // Length of the pattern P
    int cI; // Cost of INSERT
    int cD; // Cost of DELETE
    int cR; // Cost of REWRITE
    loadSettings(N, M, cI, cD, cR);


    // -- LOAD DATA -- //
    char *text = new char[N+1]();
    char *pattern = new char[M+1]();

    // Read the input
    std::scanf(" ");
    std::fread(text, sizeof(char), N, stdin);
    std::scanf(" ");
    std::fread(pattern, sizeof(char), M, stdin);


    // -- FIND THE CLOSEST SAMPLE TO THE PATTERN -- //
    PatternFinder::findPattern(text, pattern, N, M, cI, cD, cR);


    delete [] text;
    delete [] pattern;


#ifdef MEASURE_TIME
    std::cout << "TOTAL: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

	return 0;
}


