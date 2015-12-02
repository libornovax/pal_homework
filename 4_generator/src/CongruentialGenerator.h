#ifndef CONGRUENTIALGENERATOR_H
#define CONGRUENTIALGENERATOR_H


class CongruentialGenerator
{
    const int A_;
    const int C_;
    const int M_;

    int x_;

public:

    CongruentialGenerator (int A, int C, int M);
    ~CongruentialGenerator ();

    void reset (int seed);

    // Returns the next value from the sequence
    unsigned int getNext ();

    int getM () const;

};

#endif // CONGRUENTIALGENERATOR_H
