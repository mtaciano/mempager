#include "../utils/types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @details Handle pages not present in physical memory (page miss) using a
 * circular linked list containing the pages active (in memory). The page
 * chosen to be removed is the first page that is not referenced. After the
 * head is removed, the newly active page is added to the tail of the the list
 * and the now inactive page is loaded into swap
 *
 * @TODO Maybe unify duplicate code also in the FIFO page miss function
 */
linked_list_t *
clock_page_miss (page_size_t missed_page, linked_list_t *circular_list)
{
    linked_list_t *head = circular_list;
    page_size_t phys_position = circular_list->page->shift;

    while (head->page->referenced) {
        head->page->referenced = 0;
        phys_position = head->page->shift;
        g_real_mem[phys_position].referenced = 0;
        head = head->next;
    }
    phys_position = head->page->shift;
    g_real_mem[phys_position].referenced = 0;

    g_virt_mem[missed_page].active = 1;
    g_virt_mem[missed_page].shift = head->page->shift;

    head->page->active = 0;
    head->page->shift = 0;
    head->page->modified = 0;

    head->page = &g_virt_mem[missed_page];
    head = head->next;

    char *filename = malloc(FILENAME_SIZE * sizeof(*filename));
    mem_size_t first_removed_block = missed_page * BLOCK_SIZE;
    mem_size_t last_removed_block = (missed_page + 1) * BLOCK_SIZE;
    snprintf(
        filename, FILENAME_SIZE, "%s/%lldB-%lldB.txt", SWAP,
        first_removed_block, last_removed_block
    );
    remove(filename);

    memset(filename, 0, strlen(filename));
    mem_size_t first_added_block = phys_position * BLOCK_SIZE;
    mem_size_t last_added_block = (phys_position + 1) * BLOCK_SIZE;
    snprintf(
        filename, FILENAME_SIZE, "%s/%lldB-%lldB.txt", SWAP, first_added_block,
        last_added_block
    );
    FILE *file = fopen(filename, "w+");
    fprintf(file, "in swap");
    fclose(file);
    free(filename);

    return head;
}
