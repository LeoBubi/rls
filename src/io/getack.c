#include "includes.h"


rlsack_t
getack(int sockfd)
{
    /* ----- get ack from server ----- */
    
    rlsack_t ack;
    if (read(sockfd, &ack, sizeof(ack)) == -1) {
#ifdef __DEBUG
        perror("getack: read");
#endif
        return -1;
    }

    return ack;
}