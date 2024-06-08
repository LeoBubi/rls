#include "includes.h"


char CONFIG_FILE[PATH_MAX];  // configuration file path

extern char username[UNAMEMAX +1];
extern int port;
extern struct in_addr server_ip;
extern int connto;


int
rls_init(int argc, char const **argv)
{
    /* ----- configuration file checks ----- */

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
            if (strlen(argv[i+1]) < UNAMEMIN) {
                fprintf(stderr, "Minimum username length is %d.\n", UNAMEMIN);
                return 0;
            }
            if (strlen(argv[i+1]) > UNAMEMAX) {
                fprintf(stderr, "Maximum username length is %d.\n", UNAMEMAX);
                return 0;
            }
            
            strcpy(username, argv[++i]);
        }

        // check for port option
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (i+1 >= argc)
                fun_fail("No port provided.")
            if (!isint(argv[i+1]))
                fun_fail("Port number must be an integer.")
            if (atoi(argv[i+1]) < PORTMIN) {
                fprintf(stderr, "Minimum port number is %d.\n", PORTMIN);
                return 0;
            }
            if (atoi(argv[i+1]) > PORTMAX) {
                fprintf(stderr, "Maximum port number is %d.\n", PORTMAX);
                return 0;
            }
            
            port = atoi(argv[++i]);
        }

        else if (argv[i][0] == '-') {
            fprintf(stderr, "%s: invalid option.\n", argv[i]);
            return 0;
        }

        // check for detination (IP address or DNS name) argument
        else
        {
            struct hostent *host = gethostbyname(argv[i]);
            if (host == NULL) {
                fprintf(stderr, "%s: invalid IP address or DNS name.\n", argv[i]);
                return 0;
            }
            
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
        if (!config_get("USRNAME", username, UNAMEMAX+1))
            fun_fail("Failed to get username from configuration file.")
    }

    // if port not provided, get from configuration file
    if (port == 0)
    {
        char port_str[6]; // 5 digits + null terminator
        if (!config_get("SRVPORT", port_str, 6))
            fun_fail("Failed to get port from configuration file.")
        
        if (!isint(port_str))
            fun_fail("Port number in configuration file must be an integer.")
        if (atoi(port_str) < PORTMIN)
            fun_fail("Port number in configuration file too low.")
        if (atoi(port_str) > PORTMAX)
            fun_fail("Port number in configuration file too high.")
        
        port = atoi(port_str);
    }

    // get server communication delay limit from configuration file
    char connto_str[16];
    if (!config_get("CONNTMO", connto_str, 16))
        fun_fail("Failed to get server communication delay limit from configuration file.")
    
    if (!isint(connto_str))
        fun_fail("Maximum password attempts in configuration file must be an integer.")
    if (atoi(connto_str) < MINCNTO)
        fun_fail("Server communication delay limit in configuration file too low.")
    if (atoi(connto_str) > MAXCNTO)
        fun_fail("Server communication delay limit in configuration file too high.")
    
    connto = atoi(connto_str);

    return 1;
}
