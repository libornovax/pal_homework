#ifndef PATTERNFINDER_H
#define PATTERNFINDER_H

#include <vector>


typedef unsigned int uint;


class PatternFinder
{
public:

    static void findPattern (char text[], char pattern[], int N, int M, int cI, int cD, int cR);


private:

    static void fillDistanceTable_ (char text[], char pattern[], int N, int M, int cI, int cD, int cR, std::vector<uint> &edt, std::vector<char> &directions);

    static int backtrackLength_ (std::vector<char> &directions, int N, int M, int column_begin);

};

#endif // PATTERNFINDER_H
