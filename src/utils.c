#include <ctype.h>
#include <dirent.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

// Generate a random value between [0..1)
float
rand_small (void)
{
    return (double)rand() / (double)((unsigned long int)RAND_MAX + 1);
}

// Generate a normalized random value based on a given `mean` and `stddev`
double
rand_normal (double mean, double std_dev)
{
    double x = 0.;
    do {
        x = rand_small();
    } while (x == 0.);

    double y = sqrt(-2 * log(x));

    double theta = 0.;
    do {
        theta = 2 * M_PI * rand_small();
    } while (theta == 0.);

    double z = y * cos(theta);

    return (z * std_dev) + mean;
}

// Mutate string making it lowercase
void
to_lowercase (char *str)
{
    while (*str != '\0') {
        *str = tolower(*str);
        str++;
    }
}

// Check if the given string is a number or not
bool
is_number (const char *str)
{
    while (*str != '\0') {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }

    return true;
}

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
