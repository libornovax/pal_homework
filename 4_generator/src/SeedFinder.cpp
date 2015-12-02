#include "SeedFinder.h"

#ifdef MEASURE_TIME
#include <ctime>
#endif


SeedFinder::SeedFinder (std::shared_ptr<FactorizationManager> fm, std::shared_ptr<CongruentialGenerator> cg)
    : fm_(fm),
      cg_(cg)
{

}


SeedFinder::~SeedFinder ()
{

}


Seed SeedFinder::findBestSeed (int N)
{
#ifdef MEASURE_TIME
    std::clock_t clb = std::clock();
#endif
    // Generate the sequence of numbers from the generator and for each of the numbers determine, whether
    // it is the desired one that we want in the interval or not
    const int M = this->cg_->getM();

#ifdef MEASURE_TIME
    std::clock_t cl = std::clock();
#endif
    int* sequence = new int[M];
#ifdef MEASURE_TIME
    std::cout << "SeedFinder: Array creation: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    // First number is the seed 0
    sequence[0] = 0;
    this->cg_->reset(0);

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    for (int i = 1; i < M; ++i)
    {
        sequence[i] = this->cg_->getNext();
    }
#ifdef MEASURE_TIME
    std::cout << "SeedFinder: Sequence generation: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    // For some very strange reason it is about 20% faster to iterate through the array again and mark the
    // values then doing it all together???
    for (int i = 1; i < M; ++i)
    {
        if (this->fm_->hasKPrimeFactor(sequence[i]))
        {
            // Mark this number as the one we are looking for
            sequence[i] *= -1;
        }
    }
#ifdef MEASURE_TIME
    std::cout << "SeedFinder: Number lookup: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif


    // Search the sequence for the best seed (interval of length N with the highest amount of the
    // desired - marked numbers)
    Seed max_s;

    // Determine the number of the desired numbers in the first interval (with seed 0)
    for (int i = 0; i < N; ++i)
    {
        if (sequence[i] < 0)
            max_s.interval_support++;
    }

#ifdef MEASURE_TIME
    cl = std::clock();
#endif
    // Move the interval one by one and check if the support is not bigger than max
    int support = max_s.interval_support;
    for (int i = 0, i2 = N; i < M-1; ++i, ++i2)
    {
        // i2 is the leading index - we need to start from the beginnig once we get to the end of the sequence
        if (i2 >= M)
            i2 -= M;

        if (sequence[i] < 0)
            support--;

        if (sequence[i2] < 0)
            support++;

        // Check if the support is greater for this interval
        if (support > max_s.interval_support)
        {
            // We found a new interval with the highest support
            max_s.interval_support = support;
            max_s.seed = std::abs(sequence[i+1]);
        }
    }
#ifdef MEASURE_TIME
    std::cout << "SeedFinder: Seed finding: " << ((double)std::clock() - cl)/CLOCKS_PER_SEC << "s" << std::endl;
#endif


    // Free memory
    delete [] sequence;


#ifdef MEASURE_TIME
    std::cout << "SeedFinder: TOTAL: " << ((double)std::clock() - clb)/CLOCKS_PER_SEC << "s" << std::endl;
#endif

    return max_s;
}
