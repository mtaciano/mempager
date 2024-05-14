#ifndef CLOCK_H
#define CLOCK_H

#include "../utils/types.h"

// Handle page misses (virtual memory not present in physical memory) using
// a circular list to choose what pages to unload
linked_list_t *
clock_page_miss (page_size_t missed_page, linked_list_t *circular_list);

#endif
