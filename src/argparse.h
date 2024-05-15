#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "utils/types.h"

#include <stdbool.h>

/**
 * @brief The parsed arguments
 */
typedef struct {
    pager_t pager; /** Chosen pager (FIFO/CLOCK) */
    distribution_t distribution; /** Chosen distribution (UNIFORM/NORMAL) */
    bool has_seed; /** Optional seed was given */
    unsigned int seed; /** Seed value */
} args_t;

/**
 * @brief Parse the arguments given by the user
 *
 * @param argc Number of arguments
 * @param argv Value of the arguments
 *
 * @return Parsed arguments values
 */
args_t parse_args (int argc, char **argv);

#endif
