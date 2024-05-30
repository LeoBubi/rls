#include "includes.h"


int
sndctl(int sockfd, char command)
{
    char type = CTLMSG;

    // send message type to server
    if (write(sockfd, &type, sizeof(type)) == -1) {
#ifdef __DEBUG
        perror("sndctl: send message type: write");
#endif
        return 0;
    }

    // send control command to server 
    if (write(sockfd, &command, sizeof(command)) == -1) {
#ifdef __DEBUG
        perror("sndctl: send control command: write");
#endif
        return 0;
    }

    return 1;
}
