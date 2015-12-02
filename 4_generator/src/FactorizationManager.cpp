#include "FactorizationManager.h"

#include <cmath>
#include <ctime>


FactorizationManager::FactorizationManager (int M, int K)
    : M_(M),
      K_(K)
{
    // Prime numbers are special case - they actually have no factorization to prime numbers at all (except
    // for itself, which we do not count)
    if (this->K_ == 1)
        this->K_ = 0;

    this->fillNumbers_();
}


FactorizationManager::~FactorizationManager ()
{
    delete [] this->numbers_;
    delete [] this->factors_;
}


bool FactorizationManager::hasKPrimeFactor (unsigned int x)
{
#ifdef SANITY_CHECK
    if (x >= this->M_)
    {
        std::cout << "M = " << this->M_ << " | x = " << x << std::endl;
        throw std::logic_error("FactorizationManager::hasKPrimeFactor(): Number is greater tham M!");
    }
#endif

    // If x is smaller that the minimum number that could have been generated just return false
    if (x < this->min_number_)
        return false;

    // Ok, we could have generated this number -> check it
    if (this->factors_[x] == this->K_)
    {
        if (this->numbers_[x] == x)
            // Yes, this number has the desired prime factorization
            return true;
        else if (this->K_ == 0 && this->numbers_[x] == 1)
            // Prime numbers - special case
            return true;
    }

    return false;
}



// ---------------------------------------- PRIVATE METHODS ---------------------------------------- //

void FactorizationManager::fillNumbers_ ()
{
#ifdef MEASURE_TIME
    std::clock_t clb = std::clock();
#endif
    // Create the structure (vector) for determining whether a certain number belongs to the desired ones or
    // not, which will then be used for searching the interval of these numbers

    // Because we have to multiply K prime numbers in order to get the desired ones, we can determine the lowest
    // integer number that satisfies this condition - this will allow us to cut off a large part of the numbers
    // if K is high

    // First 10 prime numbers
    std::vector<unsigned int> first_10_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

    // Find the minimum number, which can be reached by multiplying the first K prime numbers
    this->min_number_ = 1;
    for (int i = 0; i < this->K_; ++i)
    {
        this->min_number_ *= first_10_primes[i];
    }


    // -- FILL NUMBERS AND FACTORS -- //
#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    // Initialize numbers with 1s and factors with 0s
    this->numbers_ = new unsigned int[this->M_];
    for (int i = 0; i < this->M_; ++i) this->numbers_[i] = 1;

    this->factors_ = new unsigned int[this->M_](); // zeros
#ifdef MEASURE_TIME
    std::cout << "FactorizationManager: Array initialization: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    // Treat 0 and 1 separately -> set there some nonsense
    this->numbers_[0] = 100; this->factors_[0] = 100;
    this->numbers_[1] = 100; this->factors_[1] = 100;

    // The maximum number that will be used for multiplication - getting the numbers multiplied by K primes
    int max_number = std::ceil(this->M_/((this->K_ == 0) ? 2 : this->min_number_/first_10_primes[this->K_-1]));

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    for (int i = 2; i <= max_number; ++i)
    {
        // If this is a prime number, multiply all the numbers that are divisible by it
        if (this->factors_[i] == 0)
        {
            // This is a prime number

            // Go through all numbers that have this prime number in their factorization
            for (int j = 2*i; j < this->M_; j += i)
            {
                this->numbers_[j] *= i;
                this->factors_[j]++;
            }
        }
    }
#ifdef MEASURE_TIME
    std::cout << "FactorizationManager: Number multiplication: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif


#ifdef MEASURE_TIME
    std::cout << "FactorizationManager: TOTAL: " << ((double)std::clock() - clb)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

}
