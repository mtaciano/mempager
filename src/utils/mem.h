#ifndef MEM_H
#define MEM_H

/**
 * @brief Reset all modifiers of all pages
 */
void reset_modifiers (void);

/**
 * @brief Initialize the pages with their contents
 */
void init_pages (void);

/**
 * @brief Initialize the swap partition
 */
void init_swap (void);

/**
 * @brief Delete the swap partition
 */
void deinit_swap (void);

#endif
