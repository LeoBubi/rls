#include "includes.h"


char CONFIG_FILE[PATH_MAX]; // configuration file path

char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address


int
main(int argc, char *argv[])
{
    // check to make sure we are not running as root
    if (getuid() == 0 || geteuid() == 0)
        main_fail("ERROR: Running as root is unnecessary and hence not allowed.")
    
    // if no arguments provided, print usage
    if (argc == 1)
    {
        printf("Usage: %s destination [-l username] [-p port]\n", argv[0]);
        return 0;
    }

    // initialize rls parameters
    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    
    printf("Username: %s\n", username);
    printf("Server IP: %s\n", inet_ntoa(server_ip));
    printf("Server port: %d\n", port);

    // connect to server
    int sockfd = rls_connect();
    if (!sockfd)
        main_fail("Cannot connect to server.")
    
    // establish user session
    if (!rls_session(sockfd)) {
        close(sockfd);
        main_fail("Cannot start new terminal session.")
    }

    // communicate with server
    int rv = rls_communicate(sockfd);

    // close connection
    close(sockfd);
    printf("Terminated remote login session.\n");

    exit(rv ? EXIT_SUCCESS : EXIT_FAILURE);
}
