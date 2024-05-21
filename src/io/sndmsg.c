#include "includes.h"


int
sndmsg(int sockfd, const char *msg)
{
    size_t size = strlen(msg) +1; // include null-terminator

    // send message type to server
    char type = DATAMSG_TYPE;
    if (write(sockfd, &type, sizeof(type)) == -1) {
#ifdef __DEBUG
        perror("sndmsg: send message type: write");
#endif
        return 0;
    }

    // send message size to server
    if (write(sockfd, &size, sizeof(size)) == -1) {
#ifdef __DEBUG
        perror("sndmsg: send message size: write");
#endif
        return 0;
    }

    // send message to server
    if (write(sockfd, msg, size) == -1) {
#ifdef __DEBUG
        perror("sndmsg: send message: write");
#endif
        return 0;
    }

    return 1;
}
