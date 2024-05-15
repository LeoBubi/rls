#include "includes.h"


int
rls_communicate(int sockfd)
{
    fd_set __readfds;
    FD_ZERO(&__readfds);
    FD_SET(STDIN_FILENO, &__readfds);
    FD_SET(sockfd, &__readfds);

    while (1)
    {
        fd_set readfds = __readfds;
        if (select(sockfd +1, &readfds, NULL, NULL, NULL) == -1) {
#ifdef __DEBUG
            perror("rls_communicate: select");
            return 0;
#else
            fun_fail("Communication error.")
#endif
        }

        // user input ready
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char *input = userinput(NULL);
            if (!input) {
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: userinput failed.\n");
#else
                fprintf(stderr, "An unexpected error occurred.\n");
#endif
                return 0;
            }

            if (write(sockfd, input, strlen(input) +1) == -1) {
                free(input);
#ifdef __DEBUG
                perror("rls_communicate: write");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            free(input);
        }

        // server response ready
        else if (FD_ISSET(sockfd, &readfds))
        {
            // MAYBE YOU SHOULD INCLUDE A FIXED HEADER TO INDICATE THE LENGTH OF THE MESSAGE
        }
    }
}
