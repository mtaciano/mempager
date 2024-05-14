#include <math.h>
#include <stdlib.h>

// Generate a random value between [0..1)
float
rand_small (void)
{
    return (double)rand() / (double)((unsigned long int)RAND_MAX + 1);
}

// Generate a normalized random value based on a given `mean` and `stddev`
double
rand_normal (double mean, double std_dev)
{
    double x = 0.;
    do {
        x = rand_small();
    } while (x == 0.);

    double y = sqrt(-2 * log(x));

    double theta = 0.;
    do {
        theta = 2 * M_PI * rand_small();
    } while (theta == 0.);

    double z = y * cos(theta);

    return (z * std_dev) + mean;
}
