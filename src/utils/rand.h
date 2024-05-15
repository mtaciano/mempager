#ifndef RAND_H
#define RAND_H

/**
 * @brief Generate an uniform random value between [0..1)
 *
 * @return The uniform random value
 */
float rand_small (void);

/**
 * @brief Generate a normalized random value
 *
 * @param mean Mean of the normal function
 * @param std_dev Standard deviation of the normal function
 *
 * @return The normalized random value
 */
double rand_normal (double mean, double std_dev);

#endif
