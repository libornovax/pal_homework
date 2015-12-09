#ifndef FACTORIZATIONMANAGER_H
#define FACTORIZATIONMANAGER_H

#include <vector>
#include <iostream>


//
// The sieve of Erastosthenes modified for finding numbers, which are multiplications of K distinct primes.
// Pregenerates all numbers from 2 to M
//
class FactorizationManager
{
    const int M_;
    int K_;
//    std::vector<unsigned int> numbers_; // too slow
//    std::vector<unsigned int> factors_; // too slow
    // If the number equals the index of the array and the factors are K, the number is the one we are looking for
    unsigned int* numbers_;
    // How many prime numbers are the numbers factorizable into
    unsigned int* factors_;
    // Lowest possible number to be factorized into K_ prime numbers
    int min_number_;


public:

    FactorizationManager (int M, int K);
    ~FactorizationManager ();

    // Tells us if the number x is the desired number (a number, which has a factorization of K
    // distinct prime numbers)
    bool hasKPrimeFactor (unsigned int x);


private:

    void fillNumbers_ ();

};

#endif // FACTORIZATIONMANAGER_H
