#include "includes.h"


char CONFIG_FILE[PATH_MAX];  // configuration file path

extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;


int
rls_initialize(int argc, char const **argv)
{
    /* ----- configuration file ----- */

    // compute configuration file path
    char *home = getenv("HOME");
    if (home == NULL)
        fun_fail("Failed to get home directory.")
    snprintf(CONFIG_FILE, sizeof(CONFIG_FILE), "%s/.config/rls/rls.conf", home);

    // check configuration file existence
    if (access(CONFIG_FILE, F_OK) == -1)
        fun_fail("Configuration file not found.")
    
    // check configuration file readability
    if (access(CONFIG_FILE, R_OK) == -1)
        fun_fail("Configuration file not readable.")

    /* ----- read command line arguments ----- */

    username[0] = '\0';     // initialize username to empty string
    port = 0;               // initialize port to 0
    server_ip.s_addr = 0;   // initialize server IP to 0
    
    for (int i = 1; i < argc; i++)
    {
        // check for username option
        if (strcmp(argv[i], "-l") == 0) 
        {
            if (i+1 >= argc)
                fun_fail("No username provided.")
            if (strlen(argv[i+1]) < UNAMEMIN)
                fun_fail("Username too short.")
            if (strlen(argv[i+1]) > UNAMEMAX)
                fun_fail("Username too long.")
            
            strcpy(username, argv[++i]);
        }

        // check for port option
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (i+1 >= argc)
                fun_fail("No port provided.")
            if (!isint(argv[i+1]))
                fun_fail("Port number must be an integer.")
            if (atoi(argv[i+1]) < PORTMIN)
                fun_fail("Port number too low.")
            if (atoi(argv[i+1]) > PORTMAX)
                fun_fail("Port number too high.")
            
            port = atoi(argv[++i]);
        }

        else if (argv[i][0] == '-')
            fun_fail("Invalid option.")

        // check for detination (IP address or hostname) argument
        else
        {
            struct hostent *host = gethostbyname(argv[i]);
            if (host == NULL)
                fun_fail("Invalid hostname or IP address.")
            
            server_ip = *(struct in_addr *)host->h_addr_list[0];
        }
        
    }

    // if destination not provided, fail
    if (server_ip.s_addr == 0)
        fun_fail("No destination provided.")

    /* ----- read configuration file ----- */

    // if username not provided, get from configuration file
    if (username[0] == '\0')
    {
        if (!config_get("USERNAME", username, UNAMEMAX+1))
            fun_fail("Failed to get username from configuration file.")
    }

    // if port not provided, get from configuration file
    if (port == 0)
    {
        char port_str[6]; // 5 digits + null terminator
        if (!config_get("PORT", port_str, 6))
            fun_fail("Failed to get port from configuration file.")
        
        if (!isint(port_str))
            fun_fail("Port number in configuration file must be an integer.")
        if (atoi(port_str) < PORTMIN)
            fun_fail("Port number in configuration file too low.")
        if (atoi(port_str) > PORTMAX)
            fun_fail("Port number in configuration file too high.")
        
        port = atoi(port_str);
    }

    return 1;
}
