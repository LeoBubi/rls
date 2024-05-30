#include "includes.h"


ack_t
getack(int sockfd)
{
    // receive ack from server
    ack_t ack;
    if (read(sockfd, &ack, sizeof(ack)) == -1) {
#ifdef __DEBUG
        perror("getack: read");
#endif
        return -1;
    }

    return ack;
}