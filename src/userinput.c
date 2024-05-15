#include "includes.h"


#define UIN_INITBUFSIZ 8


char* 
userinput(char* prompt) {
    if (prompt != NULL) {
        printf("%s", prompt);
    }

    char *input = (char*)malloc(UIN_INITBUFSIZ);
    if (!input) {
#ifdef __DEBUG
        perror("userinput: malloc");
#endif
        return NULL;
    }

    size_t input_maxsize = UIN_INITBUFSIZ;  // current maximum size of input
    size_t input_size    = 0;               // current size of input

    int c;
    while ((c = getchar()) != '\n' && c != EOF) 
    {
        input[input_size++] = c;

        if (input_size == input_maxsize)
        {
            input_maxsize *= 2;
            char *tmp = (char*)realloc(input, input_maxsize);
            if (!tmp) {
#ifdef __DEBUG
                perror("userinput: realloc");
#endif
                free(input);
                return NULL;
            }
            input = tmp;
        }
    }

    input[input_size] = '\0'; // null-terminate the input string

    return input;
}
