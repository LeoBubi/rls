#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;


int
rls_session(int sockfd)
{
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
    int ack;
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
}