#include "includes.h"


extern char CONFIG_FILE[PATH_MAX];  // configuration file path


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
    
    char *line;
    while ((line = rdline(config_fd)))
    {
        char *tok = strtok(line, "=");
        if (tok == NULL) {
            free(line);
            close(config_fd);
            fun_fail("Invalid configuration file format.")
        }
        
        if (strcmp(tok, key) == 0)
        {
            tok = strtok(NULL, "\0");
            if (tok == NULL) {
                free(line);
                close(config_fd);
                fun_fail("Invalid configuration file format: no value for specified key.")
            }
            
            if (strlen(tok) > n-1) {
                free(line);
                close(config_fd);
                fun_fail("Invalid configuration file format: value too long.")
            }
            
            close(config_fd);
            strncpy(value, tok, n);
            free(line);
            return 1;
        }
    }

    close(config_fd);
    fprintf(stderr, "%s: key not found in configuration file.\n", key);
    return 0;
}
