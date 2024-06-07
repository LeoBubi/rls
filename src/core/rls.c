#include "includes.h"


char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address

struct termios oldt, newt;   // terminal attributes


// reset terminal attributes at exit
void reset_term(void) {
    ioctl(STDIN_FILENO, TCSETS, &oldt);
}


int
main(int argc, char const *argv[])
{
    /* ----- initial checks ----- */

    if (!rls_check())
        exit(EXIT_FAILURE);
    
    if (argc == 1) {    // if no arguments provided, print usage
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /* ----- initialize rls client ----- */

    if (!rls_init(argc, argv))
        main_fail("Initialization failure.")

    printf(
        "\n"
        "Username:\t%s\n"
        "Server IP:\t%s\n"
        "Server port:\t%d\n"
        "\n",
        username, inet_ntoa(server_ip), port
    );

    /* ----- disable echo and set raw mode ----- */

    if (ioctl(STDIN_FILENO, TCGETS, &oldt) < 0) {
#ifdef __DEBUG
        fprintf(stderr, "rls: cannot get terminal attributes.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);

    if (ioctl(STDIN_FILENO, TCSETS, &newt) < 0) {
#ifdef __DEBUG
        fprintf(stderr, "rls: cannot set terminal attributes.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    /* ----- register atexit function ----- */

    atexit(reset_term);

    /* ----- connect to server ----- */

    int sockfd = rls_connect();
    if (!sockfd) {
        fprintf(stderr, "Unable to connect to server at %s:%d.\n", inet_ntoa(server_ip), port);
        exit(EXIT_FAILURE);
    }

    printf("Established connection to server.\n");
    
    /* ----- establish remote login session ----- */
    if (!rls_auth(sockfd)) {
        close(sockfd);
        main_fail("Unable to start remote login session.")
    }

    printf(
        "\n\n"
        "#####################################\n"
        "### Started remote login session. ###\n"
        "### Type ~q to quit.              ###\n"
        "#####################################\n"
        "\n"
    );

    /* ----- communicate with server ----- */

    int rv = rls_session(sockfd);

    /* ----- terminate ----- */

    close(sockfd);
    printf("Terminated remote login session.\n");

    exit(rv ? EXIT_SUCCESS : EXIT_FAILURE);
}
