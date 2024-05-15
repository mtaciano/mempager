#include "types.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @details Remove all modifiers of all pages. This is done to remove the
 * modifiers of old pages (neither referenced nor modified recently) as they
 * may be confused with more commonly accesed ones. Since pages with high hit
 * counts should gain these modifiers again this is not a problem, and may
 * reduce cache misses with algorithms that use page modifiers as a decision
 * factor for what page to remove when a page miss happens
 */
void
reset_modifiers (void)
{
    for (mem_size_t i = 0; i < VIRT_MEM_PAGES; i++) {
        if (g_virt_mem[i].active) {
            g_virt_mem[i].referenced = 0;
            g_virt_mem[i].modified = 0;

            page_size_t phys_position = g_virt_mem[i].shift;
            g_real_mem[phys_position].referenced = 0;
            g_real_mem[phys_position].modified = 0;
        }
    }
}

/**
 * @details Initialize the virtual pages as all zeroes, as they initially don't
 * carry any information. The physical pages are also initialized with zeroes
 * in all fields but their `shift`s, as this is the value of their location
 * (index) in the physical memory layout, and should never be modified
 */
void
init_pages (void)
{
    printf("(utils::mem): initializing pages.\n");

    for (int i = 0; i < VIRT_MEM_PAGES; i++) {
        g_virt_mem[i].active = 0;
        g_virt_mem[i].referenced = 0;
        g_virt_mem[i].modified = 0;
        g_virt_mem[i].padding = 0;
        g_virt_mem[i].shift = 0;
    }

    for (int i = 0; i < REAL_MEM_PAGES; i++) {
        g_real_mem[i].active = 0;
        g_real_mem[i].referenced = 0;
        g_real_mem[i].modified = 0;
        g_real_mem[i].padding = 0;
        // Mask `i` to fit into `shift`
        g_real_mem[i].shift = (uint16_t)(i & 0xFFF);
    }

    printf("(utils::mem): pages initialized.\n");
}

/**
 * @details Create the folder to be used as the swap. This function exists only
 * to be an easier way to use this program, as it would be awkward to ask the
 * user to create a swap folder before they run the program
 */
void
init_swap (void)
{
    printf("(utils::mem): creating swap folder.\n");

    mkdir(SWAP, S_IRWXU);

    printf("(utils::mem): swap folder created.\n");
}

/**
 * @details Delete the folder used as the swap. This function exists only
 * to be an easier way to use this program, as it would be awkward to let the
 * swap folder exist even after the program finishes, since it should clean
 * itself after completion
 */
void
deinit_swap (void)
{
    printf("(utils::mem): deleting swap folder.\n");

    DIR *folder = opendir(SWAP);
    struct dirent *next_file;
    char *filename = malloc(FILENAME_SIZE * sizeof *filename);

    while ((next_file = readdir(folder)) != NULL) {
        sprintf(filename, "%s/%s", SWAP, next_file->d_name);
        remove(filename);
        memset(filename, 0, strlen(filename));
    };

    rmdir(SWAP);

    free(filename);
    closedir(folder);

    printf("(utils::mem): swap folder deleted.\n");
}
