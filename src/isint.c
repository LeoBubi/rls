#include "includes.h"


int
isint(const char *s)
{
    if (s == NULL || *s == '\0')
        return 0;

    for (int i = 0; s[i] != '\0'; i++)
        if (s[i] < '0' || s[i] > '9')
            return 0;
    
    return 1;
}