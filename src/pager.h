#ifndef PAGER_H
#define PAGER_H

#include "utils/types.h"

/**
 * @brief Simulate paging virtual memory in and out of the physical memory
 * (i.e. RAM and swap)
 *
 * @param pager What pager to use (FIFO/CLOCK)
 * @param distribution_t What distribution to use (UNIFORM/NORMAL)
 */
void pager (pager_t pager, distribution_t dist);

#endif
