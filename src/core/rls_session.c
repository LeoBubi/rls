#include "includes.h"


volatile sig_atomic_t sigcode = 0;  // '= 0' for portability

// signal handler
void signal_handler(int signo) { 
    sigcode = signo;
    return;
}


int
rls_session(int sockfd)
{
    /* ----- set signal handlers ----- */

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    /* ----- communication with server ----- */

    fd_set __readfds;
    FD_ZERO(&__readfds);
    FD_SET(STDIN_FILENO, &__readfds);
    FD_SET(sockfd, &__readfds);

    rlsack_t ack;   // server ACK

    while (1)
    {
        /* ----- wait for user input or server message ----- */

        fd_set readfds = __readfds;
        if (select(sockfd +1, &readfds, NULL, NULL, NULL) == -1) 
        {
            if (errno == EINTR && sigcode) {        // if non fatal signal received
                if (!sndsig(sockfd, sigcode)) {     // send it to server
#ifdef __DEBUG
                    fprintf(stderr, "rls_session: cannot send signal to server.\n");
                    return 0;
#else
                    fun_fail("Communication error.")
#endif
                }

                ack = getack(sockfd);
                if (ack == -1) {
#ifdef __DEBUG
                    fprintf(stderr, "rls_session: cannot receive server ACK.\n");
                    return 0;
#else
                    fun_fail("Communication error.")
#endif
                }

                if (ack == 50)
                    fun_fail("Server error.")
                
                if (ack == 40)
                    fprintf(stderr, "Invalid signal.\n");
                
                // ack = 20 -> OK

                sigcode = 0; // reset signal code
            }

            continue;
        }

        /* ----- user input ----- */

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            // read one character at a time
            int c = getchar();

            // check if escape sequence
            if (c == '\x1b')    // ESC
            {
                int seq = getchar();
                if (seq == '[') 
                {
                    int cmd = getchar();
                    if (cmd == 'A')
                        c = '\x10'; // UP (Ctrl+P)
                    else if (cmd == 'B')
                        c = '\x0e'; // DOWN (Ctrl+N)
                    else if (cmd == 'C')
                        c = '\x06'; // RIGHT (Ctrl+F)
                    else if (cmd == 'D')
                        c = '\x02'; // LEFT (Ctrl+B)
                    else if (cmd == '3' && getchar() == '~')
                        c = '\x08'; // DELETE (Ctrl+H)
                    else {
                        fprintf(stderr, "Invalid escape sequence.\n");
                        continue;
                    }
                }

            }

            // check if control command
            if (c == '~')
            {
                // read control command
                int cmd = getchar();
                if (cmd == 'q') {
                    if (!sndctl(sockfd, CTLQUIT)) {
#ifdef __DEBUG
                        fprintf(stderr, "rls_session: cannot send quit command.\n");
                        return 0;
#else
                        fun_fail("Communication error.")
#endif
                    }
                    return 1;
                }

                fprintf(stderr, "Invalid command.\n");
                continue;
            }

            // send character to server
            if (!sndchr(sockfd, c)) {
#ifdef __DEBUG
                fprintf(stderr, "rls_session: cannot send user input.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            // receive server ACK
            ack = getack(sockfd);
            if (ack == -1) {
#ifdef __DEBUG
                fprintf(stderr, "rls_session: cannot receive server ACK.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            if (ack == 50) {
                printf("Server error.\n");
                return 1;
            }

            if (ack == 40) {
                printf("Malformed data.\n");
                continue;
            }

            // ACK = 20 -> OK
        }

        /* ----- server message ----- */
        
        else if (FD_ISSET(sockfd, &readfds))
        {
            char c = getchr(sockfd);
            if (c == -1) {
#ifdef __DEBUG
                fprintf(stderr, "rls_session: cannot receive server message.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
            }

            // check if server closed connection
            if (c == 0) {
                printf("Server closed connection.\n");
                return 1;
            }

            write(STDOUT_FILENO, &c, 1);
        }
    }
}
