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
    // ACK = 20: valid username
    // ACK = 40: user doesn't exist or illegal
    // ACK = 50: internal server error
    ack = getack(sockfd);
    if (ack == -1) {
#ifdef __DEBUG
        fprintf(stderr, "rls_session: cannot receive username ack.\n");
        return 0;
#else
        fun_fail("Communication error.")
#endif
    }

    if (ack == 40) {
        printf("User doesn't exist or illegal.\n");
        return 0;
    }

    if (ack == 50) {
        fprintf(stderr, "Server error.\n");
        return 0;
    }

    // ACK = 20 -> username accepted

    /* ----- send password ----- */
    while (1)
    {
        printf("\nPassword: ");
    
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
        // ACK = 20: valid password
        // ACK = 40: incorrect password, try again
        // ACK = 41: incorrect password, access denied
        // ACK = 50: internal server error
        ack = getack(sockfd);
        if (ack == -1) {
#ifdef __DEBUG
                fprintf(stderr, "rls_session: cannot receive password ack.\n");
                return 0;
#else
                fun_fail("Communication error.")
#endif
        }

        if (ack == 20)
            break;

        if (ack == 40) {
            printf("Access denied, please try again.\n");
            continue;
        }

        if (ack == 41) {
            printf("Too many attempts, access denied.\n");
            return 0;
        }

        if (ack == 50) {
            fprintf(stderr, "Server error.\n");
            return 0;
        }
    }

    return 1;  // user session established
}