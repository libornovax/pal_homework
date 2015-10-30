#ifndef BITSET2D_H
#define BITSET2D_H

#include <iostream>
#include <vector>

//#define CHECK_BIT_ACCESS


struct Bitset2D {
    unsigned int m;
    unsigned int n;
    std::vector<bool> bitarray;


    Bitset2D (unsigned int m, unsigned int n)
        : m(m),
          n(n),
          bitarray(m*n, false)
    {
    }


    // Access the value on the [i][j] position
    bool operator() (unsigned int i, unsigned int j) const
    {
#ifdef CHECK_BIT_ACCESS
        if (i >= m || j >= n)
            throw std::logic_error("Bitset2D: Bit coordinates out of bounds");
#endif
        return this->bitarray[m*i + j];
    }


    // Access the value on the [i][j] position - this function returns a reference - it is also possible
    // to change the value
    std::vector<bool>::reference operator() (unsigned int i, unsigned int j)
    {
#ifdef CHECK_BIT_ACCESS
        if (i >= m || j >= n)
            throw std::logic_error("Bitset2D: Bit coordinates out of bounds");
#endif
        return this->bitarray[m*i + j];
    }


    // Print the bitset array
    void print ()
    {
        for (int i = 0; i < this->m; ++i)
        {
            for (int j = 0; j < this->n; ++j) {
                std::cout << this->bitarray[m*i + j] << " ";
            }
        std::cout << std::endl;
        }
    }
};


#endif // BITSET2D_H
