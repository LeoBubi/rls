#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;


int
rls_connect(void)
{
    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
#ifdef __DEBUG
        perror("rls_connect: socket");
#endif
        return 0;
    }

    // connect to server
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

    // receive ACK from server
	// ACK = 0:  connection established
	// ACK > 0:  connection refused
	// ACK = -1: communication error
    int ack = getack(sockfd);
    if (ack == -1) {
        close(sockfd);
        return 0;
    }

    if (ack > 0) {
#ifdef __DEBUG
        fprintf(stderr, "rls_connect: server ACK = %d\n", ack);
#endif
        close(sockfd);
        return 0;
    }
    
    return sockfd;
}