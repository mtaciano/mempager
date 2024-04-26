#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "clock.h"
#include "fifo.h"
#include "utils.h"

typedef struct {
    const char **argv;
    pager_t pager;
    distribution_t distribution;
    bool has_seed;
    int seed;
} args_t;

// TODO: make default values be possible
static args_t
parse_args (int argc, char **argv)
{
    printf("(main): parsing args.\n");

    if (argc < 3) {
        printf("Usage: %s <FIFO|CLOCK> <UNIFORM|NORMAL> <SEED>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < argc; i++) {
        to_lowercase(argv[i]);
    }

    args_t parsed;
    parsed.argv = (const char **)argv;

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
        parsed.has_seed = true;
        parsed.seed = atoi(argv[3]);
    } else {
        printf("Option %s does not exist, exiting.\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    printf("(main): args parsed.\n");

    return parsed;
}

static void
init_pages (void)
{
    printf("(main): initializing pages.\n");

    for (int i = 0; i < VIRT_MEM_PAGES; i++) {
        g_virt_mem[i].active = 0;
        g_virt_mem[i].referenced = 0;
        g_virt_mem[i].modified = 0;
        g_virt_mem[i].padding = 0;
        g_virt_mem[i].shift = 0;
    }

    for (int i = 0; i < REAL_MEM_PAGES; i++) {
        g_real_mem[i].active = 0;
        g_real_mem[i].referenced = 0;
        g_real_mem[i].modified = 0;
        g_real_mem[i].padding = 0;
        // Mask `i` to fit into `shift`
        g_real_mem[i].shift = (uint16_t)(i & 0xFFF);
    }

    printf("(main): pages initialized.\n");
}

int
main (int argc, char **argv)
{
    args_t args = parse_args(argc, argv);

    // Inicializando `rand()` e os structs
    if (args.has_seed) {
        srand(args.seed);
    } else {
        time_t t;
        srand((unsigned int)time(&t));
    }

    init_swap();
    init_pages();

    switch (args.pager) {
    case FIFO: {
        pager_fifo(args.distribution);
    } break;
    case CLOCK: {
        pager_clock(args.distribution);
    } break;
    default:
        break;
    }

    deinit_swap();

    return EXIT_SUCCESS;
}
