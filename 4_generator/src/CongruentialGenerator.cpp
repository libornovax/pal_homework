#include "CongruentialGenerator.h"


CongruentialGenerator::CongruentialGenerator (int A, int C, int M)
    : A_(A),
      C_(C),
      M_(M)
{
    // Reset the generator with seed 0
    this->reset(0);
}


CongruentialGenerator::~CongruentialGenerator()
{

}


void CongruentialGenerator::reset (int seed)
{
    this->x_ = seed;
}


unsigned int CongruentialGenerator::getNext ()
{
    // Compute the next x = (Ax + C) mod M
    this->x_ = ((long) this->A_ * this->x_ + this->C_) % this->M_;

    return this->x_;
}


int CongruentialGenerator::getM () const
{
    return this->M_;
}
