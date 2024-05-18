#include "includes.h"


extern volatile sig_atomic_t sigcode;


int
rls_communicate(int sockfd)
{
    fd_set __readfds;
    FD_ZERO(&__readfds);
    FD_SET(STDIN_FILENO, &__readfds);
    FD_SET(sockfd, &__readfds);

    while (1)
    {
        // wait for user input or server message
        fd_set readfds = __readfds;
        if (select(sockfd +1, &readfds, NULL, NULL, NULL) == -1) 
        {
            if (errno == EINTR && sigcode) {        // if non fatal signal received
                if (!sndsig(sockfd, sigcode)) {    // send it to server
#ifdef __DEBUG
                    fprintf(stderr, "rls_communicate: cannot send signal to server.\n");
                    return 0;
#else
                    fun_fail("Communication error.")
#endif
                }

                sigcode = 0; // reset signal code
                continue;
            }

#ifdef __DEBUG
            perror("rls_communicate: select");
            return 0;
#else
            fun_fail("Communication error.")
#endif
        }

        // IF user input ready
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            // get user input
            char *input = userinput(NULL);
            if (!input) {
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: userinput failed.\n");
#else
                fprintf(stderr, "An unexpected error occurred.\n");
#endif
                return 0;
            }

            // send user input to server
            if (!sndmsg(sockfd, input)) {
                free(input);
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: cannot send user input.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            free(input);
        }

        // IF server message ready
        else if (FD_ISSET(sockfd, &readfds))
        {
            char *message = getmsg(sockfd);
            if (!message) {
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: cannot receive server message.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            if (strlen(message) == 0) {
                free(message);
                printf("Server closed connection.\n");
                return 1;
            }

            printf("%s\n", message);
            free(message);
        }
    }
}
