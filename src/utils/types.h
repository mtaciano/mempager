#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/** Size of the variable capable of representing the maximum time size */
typedef long int time_size_t;

/** Size of the variable capable of representing the maximum memory size */
typedef long long int mem_size_t;

/** Size of the variable capable of representing the maximum page size */
typedef uint16_t page_size_t;

/** Maximum size of a filename */
#define FILENAME_SIZE 64

/**
 * @brief Amount of virtual pages available for use
 *
 * @note VIRT_MEM_PAGES = REAL_MEM_PAGES + SWAP_PAGES. In the current case,
 * this means 6144 = 4096 + 2048
 */
#define VIRT_MEM_PAGES 6144

/** Upper bound of real memory pages representable by the virtual memory */
#define MAX_REAL_PAGES 4096

/** Amount of real pages available for use */
#define REAL_MEM_PAGES MAX_REAL_PAGES

/** Name of the folder used for swap pages */
#define SWAP "SWAP"

/** Time of the test run */
#define TIME_LIMIT (time_size_t)500000

/** Time to live of the ref and mod fields */
#define REF_RESET (time_size_t)500

/** Block size of the simulated file system in bytes*/
#define BLOCK_SIZE 4096

/** Maximum memory size of the simulated file system */
#define MEM_SIZE (mem_size_t)(BLOCK_SIZE * VIRT_MEM_PAGES)

/** Middle of the memory (used for normal distribution) */
#define MEM_MIDDLE (mem_size_t)(MEM_SIZE / 2)

/** Standard deviation (used for normal distribution) */
#define STD_DEV (mem_size_t)(MEM_MIDDLE / 4)

/**
 * @brief The pager types
 */
typedef enum {
    FIFO,
    CLOCK,
} pager_t;

/**
 * @brief The distribution types
 */
typedef enum {
    UNIFORM,
    NORMAL,
} distribution_t;

/**
 * @brief Representation of both a real (physical) and virtual memory page
 */
typedef struct {
    uint16_t active : 1; /** If the page is loaded into the physical memory*/
    uint16_t referenced : 1; /** If the page was read in the last quantum */
    uint16_t modified : 1; /** If the page was modified in the last quantum */
    uint16_t padding : 1; /** Padding needed to fit the data into 16 bits */
    uint16_t shift : 12; /** The location/index of the physical memory page */
} page_t;

/**
 * @brief Linked list for the physical pages
 */
typedef struct linked_list_t {
    page_t *page;
    struct linked_list_t *next;
} linked_list_t;

/** Representation of the virtual pages is RAM */
extern page_t g_virt_mem[VIRT_MEM_PAGES];

/** Representation of the real pages is RAM */
extern page_t g_real_mem[REAL_MEM_PAGES];

#endif
