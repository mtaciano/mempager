#ifndef MISC_H
#define MISC_H

#include <stdbool.h>

/**
 * @brief Lowercase the input string
 *
 * @param[in,out] str String to be lowercased
 */
void to_lowercase (char *str);

/**
 * @brief Check if the input string is composed of only numbers
 *
 * @param str Input string
 *
 * @return If string is composed of only numbers or not
 */
bool is_number (const char *str);

#endif
