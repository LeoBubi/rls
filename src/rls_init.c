#include "includes.h"


extern char username[UNAMEMAX +1];
extern int port;

/**
 * @brief Get configuration value from configuration file.
 * @param key Configuration key.
 * @param value Configuration value buffer.
 * @param n Size of the value buffer.
 * @return 1 if successful, 0 otherwise.
*/
int config_get(char *key, char *value, size_t n);


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

        // invalid option
        else {
            fprintf(stderr, "%s: ", argv[i]);
            fun_fail("invalid option.");
        }
    }

    // if username not privided, get from configuration file
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


int
config_get(char *key, char *value, size_t n)
{
    int config_fd = open(CONFIG_FILE, O_RDONLY);
    if (config_fd == -1) {
#ifdef __DEBUG
        perror("config_get: open");
        return 0;
#else
        fun_fail("Failed to open configuration file.")
#endif
    }
    
    char line[CLINMAX +1]; // +1 for null terminator
    while (rdline(line, CLINMAX+1, config_fd))
    {
        char *tok = strtok(line, "=");
        if (tok == NULL) {
            close(config_fd);
            fun_fail("Invalid configuration file format.")
        }
        
        if (strcmp(tok, key) == 0)
        {
            tok = strtok(NULL, "\0");
            if (tok == NULL) {
                close(config_fd);
                fun_fail("Invalid configuration file format: no value for specified key.")
            }
            
            if (strlen(tok) > n-1) {
                close(config_fd);
                fun_fail("Invalid configuration file format: value too long.")
            }
            
            close(config_fd);
            strncpy(value, tok, n);
            return 1;
        }
    }

    close(config_fd);
    fun_fail("Key not found in configuration file.")
}
