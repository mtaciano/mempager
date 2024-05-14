#ifndef FIFO_H
#define FIFO_H

#include "../utils/types.h"

// Handle page misses (virtual memory not present in physical memory) using
// a linked list to choose what pages to unload
linked_list_t *
fifo_page_miss (page_size_t missed_page, linked_list_t *linked_list);

#endif
