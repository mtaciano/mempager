#ifndef ARGPARSE_H
#define ARGPARSE_H

#include "utils/types.h"

#include <stdbool.h>

typedef struct {
    pager_t pager;
    distribution_t distribution;
    bool has_seed;
    unsigned int seed;
} args_t;

args_t parse_args (int argc, char **argv);

#endif
