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
        // wait for user input or server response
        fd_set readfds = __readfds;
        if (select(sockfd +1, &readfds, NULL, NULL, NULL) == -1) {
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

            // send user input size to server
            size_t input_size = strlen(input) +1; // include null-terminator
            if (write(sockfd, &input_size, sizeof(input_size)) == -1) {
                free(input);
#ifdef __DEBUG
                perror("rls_communicate: send user input size: write");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            // send user input to server
            if (write(sockfd, input, strlen(input) +1) == -1) {
                free(input);
#ifdef __DEBUG
                perror("rls_communicate: send user input: write");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            free(input);
        }

        // IF server response ready
        else if (FD_ISSET(sockfd, &readfds))
        {
            // receive response size from server
            size_t response_size;
            ssize_t rb = read(sockfd, &response_size, sizeof(response_size));
            if (rb == -1) {
#ifdef __DEBUG
                perror("rls_communicate: receive response size: read");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            // check if server closed connection
            if (rb == 0) {
                printf("Server closed connection.\n");
                return 1;
            }

            // receive response from server and print it
            char *response = (char*)malloc(response_size);
            if (!response) {
#ifdef __DEBUG
                perror("rls_communicate: malloc");
                return 0;
#else
                fun_fail("An unexpected error occurred.")
#endif
            }

            if (read(sockfd, response, response_size) == -1) {
                free(response);
#ifdef __DEBUG
                perror("rls_communicate: receive response: read");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            printf("%s\n", response);
            free(response);
        }
    }
}
