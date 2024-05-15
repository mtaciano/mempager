#include <math.h>
#include <stdlib.h>

/**
 * @details Generate an uniformly chosen random value between [0..1) by
 * dividing the result of `rand()`, which has the range [0..RAND_MAX], by
 * `RAND_MAX + 1`. This results in a clamping of the original range,
 * transforming it into the desired [0..1) range
 */
float
rand_small (void)
{
    return (float)rand() / (float)((unsigned long int)RAND_MAX + 1);
}

/**
 * @details Generate a normalized random value using the mean and standard
 * deviation as input. This function uses the Box–Muller transform to generate
 * normalized random values using uniform random values as input
 */
double
rand_normal (double mean, double std_dev)
{
    double x = 0.;
    do {
        x = (double)rand_small();
    } while (x == 0.);

    double y = sqrt(-2. * log(x));

    double theta = 0.;
    do {
        theta = 2. * M_PI * (double)rand_small();
    } while (theta == 0.);

    double z = y * cos(theta);

    return (z * std_dev) + mean;
}
