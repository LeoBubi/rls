#include "includes.h"


int
getack(int sockfd)
{
    // receive ack from server
    char ack;
    if (read(sockfd, &ack, sizeof(ack)) == -1) {
#ifdef __DEBUG
        perror("getack: read");
#endif
        return -1;
    }

    return (int)ack;
}