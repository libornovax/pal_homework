#include "PatternFinder.h"

#include <algorithm>
#include <iostream>
#include <climits>


// Careful here!! We need to put all arguments into parenthesis because the arguments might be mathematical expressions!
// Also the indexing is reversed - the columns are the first dimension and rows the second - this order is convenient
// for filling the table
#define POS2D(arr, rows, col, row) arr[(rows)*(col)+(row)]


void PatternFinder::findPattern (char text[], char pattern[], int N, int M, int cI, int cD, int cR)
{
    // Find the closest sample (considering Levenshtein distance) in the text to the pattern

    // We will compute the distance to the pattern from each starting position in the text using dynamic
    // programming (build the edit distance table)
    // Possible edits are:
    //      INSERT: left to right movement
    //      DELETE: top to bottom movement
    //      REWRITE: diagonal movement


    // -- PREPARE THE TABLE -- //
    // Edit distance table (N - length of text, M - length of pattern)
    // We will be filling out the table 'column by column'
    std::vector<uint> edt((N+1) * (M+1));
    // We need to remember from where we came to each position for backtracking (0 = top, 1 = left, 2 = diagonal)
    std::vector<char> directions((N+1) * (M+1));

    // First row is zeros
    for (int i = 0; i < N+1; ++i)
    {
        // Row 1
        POS2D(edt, M+1, i, 0) = 0;
    }

    // Compute the first column 'epsilon' of delete operations
    for (int i = 1; i < M+1; ++i)
    {
        POS2D(edt, M+1, 0, i) = POS2D(edt, M+1, 0, i-1) + cD;
        POS2D(directions, M+1, 0, i) = 0;
    }



    // -- FILL THE TABLE -- //
    // Now fill the whole table
    PatternFinder::fillDistanceTable_ (text, pattern, N, M, cI, cD, cR, edt, directions);



    // -- FIND THE MIN -- //
    // Find the minimum value in the last row
    uint min_dist = UINT_MAX;
    uint position = 0;
    uint sample_length = UINT_MAX;

    for (int i = 1; i < N+1; ++i)
    {
        // Check for min
        if (POS2D(edt, M+1, i, M) < min_dist)
        {
            // We found a new minimum
            min_dist = POS2D(edt, M+1, i, M);
            // Backtrack how this sample was created
            sample_length = PatternFinder::backtrackLength_(directions, N, M, i);
            position = i - sample_length;
        }
        else if (POS2D(edt, M+1, i, M) == min_dist)
        {
            // The value is the same as min, we need to check if the sample is not shorter
            int l = PatternFinder::backtrackLength_(directions, N, M, i);

            if (l < sample_length)
            {
                // This sample has the same Levensthein distance, but it is shorter -> we prefer that
                sample_length = l;
                position = i - sample_length;
            }
        }
    }


    // Output the info about the found sample
    std::cout << position << " " << sample_length << " " << min_dist << " " << std::endl;




    // Print the distance table
//    for (int i = 0; i < M+1; ++i)
//    {
//        for (int j = 0; j < N+1; ++j)
//        {
//            std::cout << POS2D(edt, M+1, j, i) << "\t";
//        }

//        std::cout << std::endl;
//    }
}


inline void PatternFinder::fillDistanceTable_ (char text[], char pattern[], int N, int M, int cI, int cD, int cR, std::vector<uint> &edt, std::vector<char> &directions)
{
    for (int i = 1; i < N+1; ++i)
    {
        // For each column compute all rows
        for (int j = 1; j < M+1; ++j)
        {
            // Find the operation with the lowest cost
            uint oI = POS2D(edt, M+1, i-1, j) + cI; // Insert
            uint oD = POS2D(edt, M+1, i, j-1) + cD; // Delete
            uint oR = POS2D(edt, M+1, i-1, j-1) + ((text[i-1] == pattern[j-1]) ?  0 : cR); // Rewrite

            if (oD <= oI && oD <= oR)
            {
                // Delete operation is the best
                POS2D(edt, M+1, i, j) = oD;
                POS2D(directions, M+1, i, j) = 0;
            }
            else if (oR < oD && oR <= oI)
            {
                // Rewrite is the best option
                POS2D(edt, M+1, i, j) = oR;
                POS2D(directions, M+1, i, j) = 2;
            }
            else
            {
                // We have to use insert
                POS2D(edt, M+1, i, j) = oI;
                POS2D(directions, M+1, i, j) = 1;
            }
        }
    }
}


inline int PatternFinder::backtrackLength_ (std::vector<char> &directions, int N, int M, int column_begin)
{
    int row = M;

    // Keep traversing the table of the directions until you reach the first row
    int column = column_begin;
    while (row > 0)
    {
        // Directions: (0 = top, 1 = left, 2 = diagonal)
        if (POS2D(directions, M+1, column, row) == 0)
        {
            // Delete operation
            row--;
        }
        else if (POS2D(directions, M+1, column, row) == 2)
        {
            // Rewrite operation
            column--;
            row--;
        }
        else
        {
            // Insert operation
            column--;
        }
    }

    return column_begin - column;
}


