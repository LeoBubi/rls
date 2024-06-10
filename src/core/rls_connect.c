#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;
extern int connto;


int
rls_connect(void)
{
    /* ----- create socket ----- */

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
#ifdef __DEBUG
        perror("rls_connect: socket");
#endif
        return 0;
    }

    // set reading and writing timeouts
    struct timeval tv;
    tv.tv_sec = (time_t)connto;
    tv.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv)) == -1) {
#ifdef __DEBUG
        perror("rls_connect: setsockopt for reading timeout");
#endif
        close(sockfd);
        return 0;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof(tv)) == -1) {
#ifdef __DEBUG
        perror("rls_connect: setsockopt for writing timeout");
#endif
    }

    /* ----- connect to server ----- */

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = server_ip;

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
#ifdef __DEBUG
        perror("rls_connect: connect");
#endif
        close(sockfd);
        return 0;
    }

    /* ----- get server ACK ----- */

	// 20: server ok: ready to receive username
    // 50: internal server error
    rlsack_t ack = getack(sockfd);
    if (ack == -1) {
        close(sockfd);
        return 0;
    }

    if (ack == 50)
        return 0;
    
    // ACK = 20 -> connection established
    
    return sockfd;
}
