#include "../utils/types.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Handle page misses (virtual memory not present in physical memory) using
// a linked list to choose what pages to unload
linked_list_t *
fifo_page_miss (page_size_t missed_page, linked_list_t *linked_list)
{
    linked_list_t *head = linked_list->next;
    page_t *virt_page = linked_list->page;
    page_size_t phys_position = virt_page->shift;

    g_virt_mem[missed_page].active = 1;
    g_virt_mem[missed_page].shift = virt_page->shift;

    virt_page->active = 0;
    virt_page->modified = 0;
    virt_page->shift = 0;

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

    linked_list_t *removed = linked_list;
    linked_list = linked_list->next;
    free(removed);

    while (linked_list->next != NULL) {
        linked_list = linked_list->next;
    }

    linked_list_t *new = malloc(sizeof(*new));
    new->page = &g_virt_mem[missed_page];
    new->next = NULL;
    linked_list->next = new;

    return head;
}
