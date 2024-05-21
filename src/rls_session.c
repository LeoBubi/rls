#include "includes.h"


extern char username[UNAMEMAX +1];


int
rls_session(int sockfd)
{
    int ack;   // ACK value from server

    /* ----- send username ----- */
    
    if (!sndmsg(sockfd, username)) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: cannot send username.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    // receive ACK from server
    // ACK = 0: user exists
    // ACK > 0: user doesn't exist or illegal (root)
    ack = getack(sockfd);
    if (ack == -1) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: cannot receive username ack.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    if (ack > 0) {
        printf("User doesn't exist or illegal.\n");
        return 0;
    }

    /* ----- send password ----- */
    
    char *password = userinput("Password: ");   // get password from user
    if (!password) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: userinput failed.\n");
#else
        fprintf(stderr, "An unexpected error occurred.\n");
#endif
        return 0;
    }

    if (!sndmsg(sockfd, password)) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: cannot send password.\n");
#endif
        free(password);
        return 0;
    }

    free(password);

    // receive ACK from server
    // ACK = 0: password correct
    // ACK > 0: password incorrect
    ack = getack(sockfd);
    if (ack == -1) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: cannot receive password ack.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    if (ack > 0) {
        printf("Incorrect password.\n");
        return 0;
    }

    return 1;  // user session established
}