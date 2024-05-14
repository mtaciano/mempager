#include <ctype.h>
#include <stdbool.h>

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
