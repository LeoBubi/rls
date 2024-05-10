#include "includes.h"

#define RDLINE_INITBUFSIZ 64

int
rdline(char **lineptr, size_t *n, int fd) 
{
    if (!lineptr || !n)
#ifdef __DEBUG
        fun_fail("rdline: Invalid arguments.")
#else
        return 0;
#endif

    ssize_t tb = 0;         // Total bytes read
    char *line = *lineptr;  // Line buffer

    if (!line) 
    {
        *n = RDLINE_INITBUFSIZ;
        line = (char *)malloc(*n);
        if (!line)
#ifdef __DEBUG
            fun_fail("rdline: Memory allocation failed.")
#else
            return 0;
#endif
    }

    while (read(fd, &line[tb], 1) > 0) 
    {
        tb++;

        if (line[tb - 1] == '\n')
            break;
        
        if (tb == *n - 1) {
            *n *= 2;
            char *temp = (char *)realloc(line, *n);
            if (temp == NULL) {
                free(line);
                return -1;
            }
            line = temp;
        }
    }

    if (rb == -1) {
        free(line);
        return -1;
    }

    if (tb > 0 && line[tb - 1] == '\n') {
        tb--; // Exclude newline character
    }

    line[tb] = '\0';
    *lineptr = line;
    return tb;
}