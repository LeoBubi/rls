#include "includes.h"


volatile sig_atomic_t sigcode = 0;  // '= 0' for portability

// signal handler
void signal_handler(int signo) { 
    sigcode = signo;
    return;
}


int
rls_communicate(int sockfd)
{
    /* ----- set signal handlers ----- */

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    
    sigaction(SIGINT,  &sa, NULL);
    // other signal may be added in future

    /* ----- communication with server ----- */

    fd_set __readfds;
    FD_ZERO(&__readfds);
    FD_SET(STDIN_FILENO, &__readfds);
    FD_SET(sockfd, &__readfds);

    ack_t ack;   // server ACK

    while (1)
    {
        /* ----- wait for user input or server message ----- */

        fd_set readfds = __readfds;
        if (select(sockfd +1, &readfds, NULL, NULL, NULL) == -1) 
        {
            if (errno == EINTR && sigcode) {        // if non fatal signal received
                if (!sndsig(sockfd, sigcode)) {     // send it to server
#ifdef __DEBUG
                    fprintf(stderr, "rls_communicate: cannot send signal to server.\n");
                    return 0;
#else
                    fun_fail("Communication error.")
#endif
                }

                sigcode = 0; // reset signal code
            }

            continue;
        }

        /* ----- user input ----- */

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

            // check for control commands
            if (input[0] == '~') 
            {
                // if quit command
                if (strcmp(input, "~q") == 0) {
                    free(input);
                    if (!sndctl(sockfd, CTLQUIT)) {
#ifdef __DEBUG
                        fprintf(stderr, "rls_communicate: cannot send quit command.\n");
                        return 0;
#else
                        fun_fail("Communication error.")
#endif
                    }
                    return 1;
                }

				fprintf(stderr, "%s: invalid command.", input);
				continue;
            }

            // send user input to server
            if (!sndtxt(sockfd, input, 1)) {
                free(input);
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: cannot send user input.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            free(input);

            // get server ACK
            // 20: OK
            // 50: server error
            ack = getack(sockfd);
            if (ack == -1) {
#ifdef __DEBUG
                fprintf(stderr, "rls_communicate: cannot receive server ACK.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            if (ack == 50) {
                printf("Server error.\n");
                return 0;
            }

            // ACK = 20 -> OK
        }

        /* ----- server message ----- */
        
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

            // check if server closed connection
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
