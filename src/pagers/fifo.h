#ifndef FIFO_H
#define FIFO_H

#include "../utils/types.h"

/**
 * @brief Handle pages not present in the physical memory (page miss)
 *
 * @param missed_page Index of the page not present in physical memory
 * @param linked_list Pages present in physical memory
 *
 * @return Pages present in physical memory after swap with the missing page
 */
linked_list_t *
fifo_page_miss (page_size_t missed_page, linked_list_t *linked_list);

#endif
