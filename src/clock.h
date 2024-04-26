#ifndef CLOCK_H
#define CLOCK_H

#include "utils.h"

// The function responsible for how to handle memory paging
// This implementation uses FIFO as a decision factor for what pages to
// unload from memory (the first page loaded is the first to unload)
void pager_clock (distribution_t dist);

#endif
