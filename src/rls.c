#include "includes.h"


char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number


int
main(int argc, char *argv[])
{
    if (getuid() == 0 || geteuid() == 0)
        main_fail("ERROR: Running as root is extremely dangerous and hence not allowed.")

    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    
}
