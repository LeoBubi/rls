#include "includes.h"


int
rls_check(void)
{
    /* ----- if root fail ----- */

    if (getuid() == 0 || geteuid() == 0)
        fun_fail("Running as root is unnecessary and hence not allowed.")
    
    /* ----- all checks passed ----- */

    return 1;
}
