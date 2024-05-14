#ifndef MEM_H
#define MEM_H

// Reset all pages modifiers
// This is to clear any pages that have modifiers set but will not be in the
// future. Since pages with high hit counts should gain these modifiers again
// this is not a problem
void reset_modifiers (void);

void init_pages (void);

// Initialize (create) the `SWAP` folder
void init_swap (void);

// Remove all the leftover pages present in the `SWAP` folder
void deinit_swap (void);

#endif
