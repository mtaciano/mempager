#include "argparse.h"
#include "pager.h"
#include "utils/mem.h"
#include "utils/types.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

page_t g_virt_mem[VIRT_MEM_PAGES];
page_t g_real_mem[REAL_MEM_PAGES];

int
main (int argc, char **argv)
{
    args_t args = parse_args(argc, argv);

    // If a seed was provided, we should use it. If not, just `time()`
    if (args.has_seed) {
        srand(args.seed);
    } else {
        time_t t;
        srand((unsigned int)time(&t));
    }

    init_swap();
    init_pages();

    pager(args.pager, args.distribution);

    deinit_swap();

    return EXIT_SUCCESS;
}
