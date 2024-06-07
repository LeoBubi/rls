#include "includes.h"


char
getchr(int sockfd)
{
    /* ----- get character from server ----- */
    
    char c;
    ssize_t rb = read(sockfd, &c, 1);
    if (rb == -1) {
#ifdef __DEBUG
        perror("getchr: read");
#endif
        return -1;
    }

    if (rb == 0)
        c = 0;  // server closed connection

    return c;
}