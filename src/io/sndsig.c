#include "includes.h"


int
sndsig(int sockfd, char signo)
{
    char type = SIGMSG;

    // send message type to server
    if (write(sockfd, &type, sizeof(type)) == -1) {
#ifdef __DEBUG
        perror("sndsig: send message type: write");
#endif
        return 0;
    }

    // send signal number to server
    if (write(sockfd, &signo, sizeof(signo)) == -1) {
#ifdef __DEBUG
        perror("sndsig: send control signo: write");
#endif
        return 0;
    }

    return 1;
}
