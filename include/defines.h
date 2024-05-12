#ifndef _DEFINES_H
#define _DEFINES_H


#define CONFIG_FILE "~/.config/rls/rls.conf"

#define UNAMEMIN 1      // minimum username length
#define UNAMEMAX 32     // maximum username length
#define PORTMIN  1      // minimum server port number
#define PORTMAX  65535  // maximum server port number


#define main_fail(msg) { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); }
#define fun_fail(msg)  { fprintf(stderr, "%s\n", msg); return 0; }


/**
 * @brief Initial checks and setup.
 * @param argc Command line argument count.
 * @param argv Command line arguments.
 * @return 1 if successful, 0 otherwise.
*/
int rls_init(int argc, char **argv);


/**
 * @brief Read a line of text from a file descriptor.
 * @param line Buffer to store the line.
 * @param n Size of the buffer.
 * @param fd File descriptor.
 * @return 1 if successful, 0 otherwise.
*/
int rdline(char *line, size_t n, int fd);


#endif /* _DEFINES_H */
