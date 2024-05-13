#include "includes.h"


char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number
struct in_addr server_ip;    // server IP address


int
main(int argc, char *argv[])
{
    // check to make sure we are not running as root
    if (getuid() == 0 || geteuid() == 0)
        main_fail("ERROR: Running as root is extremely dangerous and hence not allowed.")

    // initialize rls parameters
    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    

}
