#include "includes.h"


char CONFIG_FILE[PATH_MAX]; // configuration file path

char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address


int
main(int argc, char *argv[])
{
    // check to make sure we are not running as root
    if (getuid() == 0 || geteuid() == 0)
        main_fail("ERROR: Running as root is unnecessary and hence not allowed.")

    // initialize rls parameters
    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    
    printf("Username: %s\n", username);
    printf("Server IP: %s\n", inet_ntoa(server_ip));
    printf("Server port: %d\n", port);



    return 0;
}
