#include "includes.h"


int
sndctrl(int sockfd, char code)
{
    // send message type to server
    char type = CTRLMSG_TYPE;
    if (write(sockfd, &type, sizeof(type)) == -1) {
#ifdef __DEBUG
        perror("sndctrl: send message type: write");
#endif
        return 0;
    }

    // send control code to server
    if (write(sockfd, &code, sizeof(code)) == -1) {
#ifdef __DEBUG
        perror("sndctrl: send control code: write");
#endif
        return 0;
    }

    return 1;
}
