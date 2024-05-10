#include "includes.h"


char username[UNAMEMAX +1];  // +1 for null terminator
int port;                    // server port number


int
main(int argc, char *argv[])
{
    if (!rls_init(argc, argv))
        main_fail("Failed to initialize.")
    
}
