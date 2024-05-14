#ifndef RAND_H
#define RAND_H

// Generate a random value between [0..1)
float rand_small (void);

// Generate a normalized random value based on a given `mean` and `stddev`
double rand_normal (double mean, double std_dev);

#endif
