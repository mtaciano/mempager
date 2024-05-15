#include <ctype.h>
#include <stdbool.h>

/**
 * @details Lowercase the input string, mutating it in place.
 */
void
to_lowercase (char *str)
{
    while (*str != '\0') {
        *str = (char)tolower(*str);
        str++;
    }
}

/**
 * @details Check if the input string is composed of only numbers. The string
 * is a number if all the letters are digits. As such, strings that contain a
 * mix of numbers and other characters are not considered numbers, and return
 * `false` as the result
 */
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
