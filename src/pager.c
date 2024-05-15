#include "pagers/clock.h"
#include "pagers/fifo.h"
#include "utils/mem.h"
#include "utils/rand.h"
#include "utils/types.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Free all the linked list allocations
 *
 * @details Free all the linked list allocations based on the size of
 * `REAL_MEM_PAGES`, since we know that the only linked list used is this
 * program is of this size. Therefore, the list will always have
 * `REAL_MEM_PAGES` elements in it
 *
 * @param linked_list List to be freed
 */
static void
free_linked_list (linked_list_t *linked_list)
{
    printf("(pager): freeing linked list.\n");

    linked_list_t *head = linked_list;
    for (page_size_t i = 0; i < REAL_MEM_PAGES; i++) {
        linked_list = linked_list->next;
        free(head);
        head = linked_list;
    }

    printf("(pager): linked list freed.\n");
}

/**
 * @brief Initialize the memory with a random state
 *
 * @details Randomly choose virtual pages to allocate into the physical memory
 * and fill a linked list based on that order. When the physical memory is
 * filled, allocate the rest of the pages into the swap (represented as a
 * folder in the current working directory)
 *
 * @param pager Pager being used
 *
 * @return Virtual pages loaded in physical memory
 */
static linked_list_t *
init_mem (pager_t pager)
{
    printf("(pager): initializing memory.\n");

    linked_list_t *head = NULL;
    linked_list_t *tail = NULL;

    for (page_size_t i = 0; i < REAL_MEM_PAGES; i++) {
        page_size_t chosen;
        do {
            chosen = (page_size_t)(rand() % VIRT_MEM_PAGES);
        } while (g_virt_mem[chosen].active);

        g_real_mem[i].active = 1;
        g_virt_mem[chosen].active = g_real_mem[i].active;
        g_virt_mem[chosen].shift = g_real_mem[i].shift;

        linked_list_t *new = malloc(sizeof(*new));
        new->page = &g_virt_mem[chosen];
        new->next = head;
        head = new;

        if (pager == CLOCK && tail == NULL) {
            tail = new;
        }
    }

    if (pager == CLOCK) {
        tail->next = head;
    }

    for (page_size_t i = 0; i < VIRT_MEM_PAGES; i++) {
        if (!g_virt_mem[i].active) {
            char *filename = malloc(FILENAME_SIZE * sizeof(*filename));
            mem_size_t first_block = i * BLOCK_SIZE;
            mem_size_t last_block = (i + 1) * BLOCK_SIZE;
            snprintf(
                filename, FILENAME_SIZE, "%s/%lldB-%lldB.txt", SWAP,
                first_block, last_block
            );

            FILE *file = fopen(filename, "w");
            fprintf(file, "in swap");
            fclose(file);
            free(filename);
        }
    }

    printf("(pager): memory initialized.\n");

    return head;
}

/**
 * @details Simulate `TIME_LIMIT` times a random page being chosen to be
 * referenced, and maybe modified. After `REF_RESET` rounds, reset all page
 * modifiers. When the page is not active (not loaded into physical memory), it
 * has to be removed from the swap and added to the physical memory, which
 * consequently removes another page from the physical memory and inserts it
 * into the swap, as the physical memory is supposed to be always full. The
 * choise of page to be removed depends on the pager used
 */
void
pager (pager_t pager, distribution_t dist)
{
    printf("(pager): starting.\n");

    long int page_misses = 0;
    long int page_hits = 0;
    linked_list_t *ll = init_mem(pager);

    printf("(pager): starting paging simulation.\n");

    for (time_size_t i = 0; i < TIME_LIMIT; i++) {
        if (i % REF_RESET == 0) {
            reset_modifiers();
        }

        mem_size_t address;
        switch (dist) {
        case UNIFORM: {
            address = rand() % MEM_SIZE;
        } break;
        case NORMAL: {
            address = labs((long int)round(rand_normal(MEM_MIDDLE, STD_DEV)));
            address = address % MEM_SIZE;
        } break;
        default: {
            printf("(pager): distribution not recognized, exiting.");
            exit(EXIT_FAILURE);
        } break;
        }

        page_size_t page_hit_idx = (page_size_t)(address / BLOCK_SIZE);

        if (g_virt_mem[page_hit_idx].active) {
            page_size_t phys_position = g_virt_mem[page_hit_idx].shift;

            g_virt_mem[page_hit_idx].referenced = 1;
            g_real_mem[phys_position].referenced = 1;

            // 50:50 chance of the page being modified
            if (rand() & 1) {
                g_virt_mem[page_hit_idx].modified = 1;
                g_real_mem[phys_position].modified = 1;
            }

            page_hits += 1;
        } else {
            if (pager == FIFO) {
                ll = fifo_page_miss(page_hit_idx, ll);
            } else if (pager == CLOCK) {
                ll = clock_page_miss(page_hit_idx, ll);
            }
            page_misses += 1;
        }
    }

    printf("(pager): paging simulation finished.\n");

    free_linked_list(ll);

    printf("(pager): finished.\n");

    printf("\n");
    printf("(results): simulation cycles is %ld\n", TIME_LIMIT);
    printf("(results): modifiers time to live is %ld\n", REF_RESET);
    printf("(results): total page hits is %ld\n", page_hits);
    printf("(results): total page misses is %ld\n", page_misses);
    printf("\n");
}
