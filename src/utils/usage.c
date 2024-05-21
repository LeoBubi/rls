#include "includes.h"


void usage(const char *prog)
{
    printf(
        "Usage:\t%s destination [-l username] [-p port]\n"
        "\n"
        "<destination>:\tServer IP address or hostname\n"
        "\n"
        "Options:\n"
        "\n"
        "\t-l username:\tUsername to login with\n"
        "\t-p port:\tServer port number\n"
        "\n"
        , prog);
    return;
}
