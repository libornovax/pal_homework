#ifndef SEEDFINDER_H
#define SEEDFINDER_H

#include <memory>

#include "FactorizationManager.h"
#include "CongruentialGenerator.h"


struct Seed
{
    Seed ()
        : seed(0),
          interval_support(0)
    {
    }

    int seed;
    int interval_support;
};


class SeedFinder
{
    std::shared_ptr<FactorizationManager> fm_;
    std::shared_ptr<CongruentialGenerator> cg_;

public:

    SeedFinder (std::shared_ptr<CongruentialGenerator> cg);
    ~SeedFinder ();

    // Finds the best desired seed for the interval of length N
    Seed findBestSeed (int N, int K);

};

#endif // SEEDFINDER_H
