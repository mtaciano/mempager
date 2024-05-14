#include "types.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Reset all pages modifiers
// This is to clear any pages that have modifiers set but will not be in the
// future. Since pages with high hit counts should gain these modifiers again
// this is not a problem
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

void
init_pages (void)
{
    printf("(main): initializing pages.\n");

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

    printf("(main): pages initialized.\n");
}

// Initialize (create) the `SWAP` folder
void
init_swap (void)
{
    printf("(utils): creating swap folder.\n");

    mkdir(SWAP, S_IRWXU);

    printf("(utils): swap folder created.\n");
}

// Remove all the leftover pages present in the `SWAP` folder
void
deinit_swap (void)
{
    printf("(utils): deleting swap folder.\n");

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

    printf("(utils): swap folder deleted.\n");
}
