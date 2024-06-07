#include "includes.h"


int
sndchr(int sockfd, char c)
{
    char type = CHRMSG;

    /* ----- send message type to server ----- */

    if (write(sockfd, &type, sizeof(type)) == -1) {
#ifdef __DEBUG
        perror("sndchar: send message type: write");
#endif
        return 0;
    }

    /* ----- send character to server ----- */
    
    if (write(sockfd, &c, 1) == -1) {
#ifdef __DEBUG
        perror("sndchar: send character: write");
#endif
        return 0;
    }

    return 1;
}
