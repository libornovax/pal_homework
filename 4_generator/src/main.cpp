//
// Libor Novak
// 24.11.2015
//
// PAL Homework 4 - Linear Congruential Generator
//

#include <iostream>
#include <cstdio>
#include <vector>
#include <memory>

#include "SeedFinder.h"


void loadData (int &A, int &C, int &M, int &K, int &N)
{
    // Read the settings of the congruential generator
    std::cin >> A >> C >> M >> K >> N;
}


int main (int argc, char* argv[])
{
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    // -- LOAD DATA -- //
    int A; // Generator parameter
    int C; // Generator parameter
    int M; // Generator parameter - mod (M-1 is the max number produced by the generator)
    int K; // Number of prime numbers to combine to get the desired numbers
    int N; // Length of interval
    loadData(A, C, M, K, N);


    // -- FIND THE SEED -- //
    // Find the seed for the interval with the highest number of the desired numbers
    // Create the generator
    std::shared_ptr<CongruentialGenerator> cg = std::make_shared<CongruentialGenerator>(A, C, M);

    SeedFinder sf(cg);
    // Run the seed finding procedure
    Seed s = sf.findBestSeed(N, K);

#ifdef MEASURE_TIME
    std::cout << "TOTAL: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    std::cout << s.seed << " " << s.interval_support << std::endl;


	return 0;
}


