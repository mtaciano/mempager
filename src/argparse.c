#include "argparse.h"
#include "utils/misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: make default values be possible
args_t
parse_args (int argc, char **argv)
{
    printf("(argparse): parsing args.\n");

    if (argc < 3) {
        printf("Usage: %s <FIFO|CLOCK> <UNIFORM|NORMAL> <SEED>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; i++) {
        to_lowercase(argv[i]);
    }

    args_t parsed;

    if (strcmp(argv[1], "fifo") == 0) {
        parsed.pager = FIFO;
    } else if (strcmp(argv[1], "clock") == 0) {
        parsed.pager = CLOCK;
    } else {
        printf("Option %s does not exist, exiting.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[2], "uniform") == 0) {
        parsed.distribution = UNIFORM;
    } else if (strcmp(argv[2], "normal") == 0) {
        parsed.distribution = NORMAL;
    } else {
        printf("Option %s does not exist, exiting.\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    if (argc < 4) {
        parsed.has_seed = false;
        parsed.seed = 0;
    } else if (is_number(argv[3])) {
        // The seed is always assumed to be in base 10
        parsed.has_seed = true;
        parsed.seed = atol(argv[3]);
    } else {
        printf("Option %s does not exist, exiting.\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    printf("(main): args parsed.\n");

    return parsed;
}
