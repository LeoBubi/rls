#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;


int
rls_session(int sockfd)
{
    int ack;   // ACK value from server

    // send username to server
    if (write(sockfd, username, strlen(username) +1) == -1) {
#ifdef __DEBUG
        perror("rls_session: send username: write");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    // receive ACK from server
    // ACK = 1: user exists
    // ACK = 0: user doesn't exist or illegal (root)
    if (read(sockfd, &ack, sizeof(ack)) == -1) {
#ifdef __DEBUG
        perror("rls_session: receive username ack: read");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    if (ack == 0) {
        printf("Illegal user.\n");
        return 0;
    }

    else if (ack != 1) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: Illegal ACK value from server.\n");
#endif
        return 0;
    }

    // send password to server
    char *password = userinput("Password: ");   // get password from user
    if (!password) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: userinput failed.\n");
#endif
        return 0;
    }

    if (write(sockfd, password, strlen(password) +1) == -1) {
        free(password);
#ifdef __DEBUG
        perror("rls_session: send password: write");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    free(password);

    // receive ACK from server
    // ACK = 1: password correct
    // ACK = 0: password incorrect
    if (read(sockfd, &ack, sizeof(ack)) == -1) {
#ifdef __DEBUG
        perror("rls_session: receive password ack: read");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    if (ack == 0) {
        printf("Incorrect password.\n");
        return 0;
    }

    else if (ack != 1) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: Illegal ACK value from server.\n");
#endif
        return 0;
    }

    return 1;  // user session established
}