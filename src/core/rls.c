#include "includes.h"


char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address


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

    if (!rls_initialize(argc, argv))
        main_fail("Initialization failure.")

    printf(
        "\n"
        "Username:\t%s\n"
        "Server IP:\t%s\n"
        "Server port:\t%d\n"
        "\n",
        username, inet_ntoa(server_ip), port
    );

    /* ----- connect to server ----- */

    int sockfd = rls_connect();
    if (!sockfd) {
        fprintf(stderr, "Unable to connect to server at %s:%d.\n", inet_ntoa(server_ip), port);
        exit(EXIT_FAILURE);
    }

    printf("Established connection to server.\n");
    
    // establish remote login session
    if (!rls_session(sockfd)) {
        close(sockfd);
        main_fail("Unable to start remote login session.")
    }

    printf(
        "\n"
        "#####################################"
        "### Started remote login session. ###"
        "### Type ~q to quit.              ###"
        "### Type ~h for help.             ###"
        "#####################################"
        "\n\n"
    );

    /* ----- communicate with server ----- */

    int rv = rls_communicate(sockfd);

    /* ----- terminate ----- */

    close(sockfd);
    printf("Terminated remote login session.\n");

    exit(rv ? EXIT_SUCCESS : EXIT_FAILURE);
}
