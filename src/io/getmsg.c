#include "includes.h"


char*
getmsg(int sockfd)
{
    /* ----- receive message size from server ----- */
    
    ssize_t rb;
    size_t size;

	rb = read(sockfd, &size, sizeof(size));
    if (rb == -1) {
#ifdef __DEBUG
        perror("getmsg: receive message size: read");
#endif
        return NULL;
    }

	// if server closed connection return empty string
	if (rb == 0) {
		char *msg = (char*)malloc(1);
		if (!msg) {
#ifdef __DEBUG
			perror("getmsg: malloc");
#endif
			return NULL;
		}

		msg[0] = '\0';
		return msg;
	}

    /* ----- receive message from server ----- */

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
