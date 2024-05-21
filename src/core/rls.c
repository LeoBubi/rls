#include "includes.h"


char CONFIG_FILE[PATH_MAX];  // configuration file path

char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address


volatile sig_atomic_t sigcode = 0; // signal code

// signal handler
void signal_handler(int signo) { 
    sigcode = signo;
    return;
}

int
main(int argc, char const *argv[])
{
    /* ----- initial checks ----- */

    if (!rls_checks())
        exit(EXIT_FAILURE);
    
    if (argc == 1) {    // if no arguments provided, print usage
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* ----- initialize rls client ----- */

    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    
    printf("Username:\t%s\n", username);
    printf("Server IP:\t%s\n", inet_ntoa(server_ip));
    printf("Server port:\t%d\n", port);

    /* ----- set signal handlers ----- */

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    
    sigaction(SIGINT,  &sa, NULL);
    // other signal may be added in future

    /* ----- connect to server ----- */

    int sockfd = rls_connect();
    if (!sockfd)
        main_fail("Cannot connect to server.")

    printf("Connection to server established...\n");
    
    // establish user session
    if (!rls_session(sockfd)) {
        close(sockfd);
        main_fail("Cannot start new terminal session.")
    }

    printf("...started user session.\n\n");

    /* ----- communicate with server ----- */

    int rv = rls_communicate(sockfd);

    /* ----- terminate ----- */

    close(sockfd);
    printf("Terminated remote login session.\n");

    exit(rv ? EXIT_SUCCESS : EXIT_FAILURE);
}
