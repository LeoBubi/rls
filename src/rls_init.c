#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;

/**
 * @brief Get configuration value from configuration file.
 * @param key Configuration key.
 * @param value Configuration value.
 * @return 1 if successful, 0 otherwise.
*/
int config_get(char *key, char *value);

int
rls_init(int argc, char **argv)
{
    // check configuration file existence
    if (access(CONFIG_FILE, F_OK) == -1)
        fun_fail("Configuration file not found.")
    
    // check configuration file readability
    if (access(CONFIG_FILE, R_OK) == -1)
        fun_fail("Configuration file not readable.")
    
    username[0] = '\0'; // initialize username to empty string
    port = 0;           // initialize port to 0
    
    // check command line options
    for (int i = 1; i < argc; i++)
    {
        // check for username option
        if (strcmp(argv[i], "-l") == 0) 
        {
            if (i + 1 >= argc)
                fun_fail("No username provided.")
            if (strlen(argv[i + 1]) < UNAMEMIN)
                fun_fail("Username too short.")
            if (strlen(argv[i + 1]) > UNAMEMAX)
                fun_fail("Username too long.")
            
            strcpy(username, argv[i + 1]);
        }

        // check for port option
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (i + 1 >= argc)
                fun_fail("No port provided.")
            if (atoi(argv[i + 1]) < PORTMIN)
                fun_fail("Port number too low.")
            if (atoi(argv[i + 1]) > PORTMAX)
                fun_fail("Port number too high.")
            
            port = atoi(argv[i + 1]);
        }

        // invalid option
        else
            fun_fail("Invalid option.");
        
        i++; // skip next argument
    }

    // if username not privided, get from configuration file
    if (username[0] == '\0')
    {
        int config_fd = open(CONFIG_FILE, O_RDONLY);
        if (config_fd == -1)
            fun_fail("Failed to open configuration file.")
        

}


int
config_get(char *key, char *value)
{
    int config_fd = open(CONFIG_FILE, O_RDONLY);
    if (config_fd == -1)
        fun_fail("Failed to open configuration file.")
    
    // C'È DA VEDERE COME LEGGERE IL FILE DI CONFIGURAZIONE
}
