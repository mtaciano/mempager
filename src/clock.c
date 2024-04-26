#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef linked_list_t circular_list_t;

// Free all the circular list allocations
static void
free_circular_list (circular_list_t *circular_list)
{
    printf("(pager-clock): freeing linked list.\n");

    circular_list_t *head = circular_list;
    for (page_size_t i = 0; i < REAL_MEM_PAGES; i++) {
        circular_list = circular_list->next;
        free(head);
        head = circular_list;
    }

    printf("(pager-clock): linked list freed.\n");
}

// Escolhe aleatóriamente páginas para carregar na memória real
// e preenche a lista encadeada com a ordem de inserção
static circular_list_t *
init_mem (void)
{
    printf("(pager-clock): initializing memory.\n");

    circular_list_t *head = NULL;
    circular_list_t *tail = NULL;

    for (page_size_t i = 0; i < REAL_MEM_PAGES; i++) {
        page_size_t chosen;
        do {
            chosen = rand() % VIRT_MEM_PAGES;
        } while (g_virt_mem[chosen].active);

        g_real_mem[i].active = 1;
        g_virt_mem[chosen].active = g_real_mem[i].active;
        g_virt_mem[chosen].shift = g_real_mem[i].shift;

        circular_list_t *new = malloc(sizeof(*new));
        new->page = &g_virt_mem[chosen];
        new->next = head;
        head = new;
        if (tail == NULL) {
            tail = new;
        }
    }

    tail->next = head;

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

    printf("(pager-clock): memory initialized.\n");

    return head;
}

// Handle page misses (virtual memory not present in physical memory) using
// a circular list to choose what pages to unload
static circular_list_t *
page_miss (page_size_t missed_page, circular_list_t *circular_list)
{
    circular_list_t *head = circular_list;
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

// The function responsible for how to handle memory paging
// This implementation uses FIFO as a decision factor for what pages to
// unload from memory (the first page loaded is the first to unload)
void
pager_clock (distribution_t dist)
{
    printf("(pager-clock): starting.\n");

    long int page_misses = 0;
    long int page_hits = 0;
    circular_list_t *cl = init_mem();

    printf("(pager-clock): starting paging simulation.\n");

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
            printf("(pager-fifo): distribution not recognized, exiting.");
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
            cl = page_miss(page_hit_idx, cl);
            page_misses += 1;
        }
    }

    printf("(pager-clock): paging simulation finished.\n");

    free_circular_list(cl);

    printf("(pager-clock): finished.\n");

    printf("\n");
    printf("(results): simulation cycles is %ld\n", TIME_LIMIT);
    printf("(results): modifiers time to live is %ld\n", REF_RESET);
    printf("(results): total page hits is %ld\n", page_hits);
    printf("(results): total page misses is %ld\n", page_misses);
    printf("\n");
}