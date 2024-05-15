#include "includes.h"


char*
getmsg(int sockfd)
{
    // receive message size from server
    size_t size;
    if (read(sockfd, &size, sizeof(size)) == -1) {
#ifdef __DEBUG
        perror("getmsg: receive message size: read");
#endif
        return NULL;
    }

    // receive message from server
    char *msg = (char*)malloc(size);
    if (!msg) {
#ifdef __DEBUG
        perror("getmsg: malloc");
#endif
        return NULL;
    }

    if (read(sockfd, msg, size) == -1) {
#ifdef __DEBUG
        perror("getmsg: receive message: read");
#endif
        free(msg);
        return NULL;
    }

    return msg;
}
