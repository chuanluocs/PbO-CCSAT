#ifndef _MERSENNE_H
#define _MERSENNE_H

class Mersenne
{
    static const int N = 624;
    unsigned int mt[N];
    int mti;

public:
    Mersenne();                               // seed with time-dependent value
    Mersenne(int seed);                       // seed with int value; see comments for the seed() method
    Mersenne(unsigned int *array, int count); // seed with array
    Mersenne(const Mersenne &copy);
    Mersenne &operator=(const Mersenne &copy);

    void seed(int s);
    void seed(unsigned int *array, int len);

    unsigned int next32(); // generates random integer in [0..2^32-1]
    int next31();          // generates random integer in [0..2^31-1]
    double nextClosed();   // generates random float in [0..1], 2^53 possible values
    double nextHalfOpen(); // generates random float in [0..1), 2^53 possible values
    double nextOpen();     // generates random float in (0..1), 2^53 possible values
    int next(int bound);   // generates random integer in [0..bound), bound < 2^31
};

#endif

/*
  Notes on seeding

  1. Seeding with an integer
     To avoid different seeds mapping to the same sequence, follow one of
     the following two conventions:
     a) Only use seeds in 0..2^31-1     (preferred)
     b) Only use seeds in -2^30..2^30-1 (2-complement machines only)

  2. Seeding with an array (die-hard seed method)
     The length of the array, len, can be arbitrarily high, but for lengths greater
     than N, collisions are common. If the seed is of high quality, using more than
     N values does not make sense.
*/
