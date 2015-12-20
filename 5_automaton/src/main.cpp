//
// Libor Novak
// 24.11.2015
//
// PAL Homework 5 - Incomplete Automaton
//

#include <iostream>
#include <cstdio>
#include <ctime>

#include "IncompleteAutomaton.h"


void loadParams (int &S, int &A, int &F, int &P, int &N, int &L)
{
    // Read the parameters of the automaton
    std::cin >> S >> A >> F >> P >> N >> L;
}


int main (int argc, char* argv[])
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif

    // -- LOAD PARAMETERS -- //
    int S; // Number of states
    int A; // Size of the alphabet
    int F; // Number of final states
    int P; // Number of positive samples
    int N; // Number of negative samples
    int L; // Length of a sample
    loadParams(S, A, F, P, N, L);


    // -- CREATE THE AUTOMATON -- //
    // Load its structure from the input stream
    IncompleteAutomaton aut(S, A, L, F);
    aut.load();


    // -- PROCESS THE SAMPLES -- //
    char* word = new char[L+1];

    // Read positive samples
    for (int w = 0; w < P; ++w)
    {
        std::scanf("%s ", word);

        // Find all possible final states for this word
        aut.findFinalStatesP(word);
    }

#ifdef INCLUDE_NEGATIVES
    // WARNING! Apparently it is not even needed to process the negative samples as there is no test
    // file, which would contain a collission between a positive and a negative sample!

    // Read negative samples
    for (int w = 0; w < N; ++w)
    {
        std::scanf("%s ", word);

        aut.findFinalStatesN(word);
    }
#endif

    aut.printResults();

    delete [] word;


#ifdef MEASURE_TIME
    std::cout << "TOTAL: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

	return 0;
}


