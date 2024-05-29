#ifndef _DEFINES_H
#define _DEFINES_H


extern char CONFIG_FILE[PATH_MAX]; // configuration file path

#define UNAMEMIN 1      // minimum username length
#define UNAMEMAX 32     // maximum username length
#define PORTMIN  1      // minimum server port number
#define PORTMAX  65535  // maximum server port number

#define CLINMAX 128 // maximum line length in configuration file


#define main_fail(msg) { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); }
#define fun_fail(msg)  { fprintf(stderr, "%s\n", msg); return 0; }

#define TXTMSG 0  // text message type code
#define SIGMSG 1  // signal message type code


/**
 * @brief Initial checks and setup.
 * @param argc Command line argument count.
 * @param argv Command line arguments.
 * @return 1 if successful, 0 otherwise.
*/
int rls_initialize(int argc, char const **argv);


/**
 * @brief Read a line of text from a file descriptor.
 * @param line Buffer to store the line.
 * @param n Size of the buffer.
 * @param fd File descriptor.
 * @return 1 if successful, 0 otherwise.
*/
int rdline(char *line, size_t n, int fd);


/**
 * @brief Check if a string is an integer.
 * @param s String to check.
 * @return 1 if integer, 0 otherwise.
*/
int isint(const char *s);


/**
 * @brief Connect to server.
 * @return Socket file descriptor if successful, 0 otherwise.
*/
int rls_connect(void);


/**
 * @brief Start a terminal session on the server.
 * @param sockfd Socket file descriptor.
 * @return 1 if successful, 0 otherwise.
*/
int rls_session(int sockfd);


/**
 * @brief Get user input.
 * @param prompt Prompt message or NULL.
 * @return User input string w/o newline character.
 * @note Returned string must be freed by the caller.
*/
char* userinput(char* prompt);


/**
 * @brief Communicate with server.
 * @param sockfd Socket file descriptor.
 * @return 1 if successful, 0 otherwise.
*/
int rls_communicate(int sockfd);


/**
 * @brief Get message from server.
 * @param sockfd Socket file descriptor.
 * @return Message string if successful, an empty string if server closed connection, or NULL otherwise.
 * @note Returned string must be freed by the caller.
*/
char* getmsg(int sockfd);


/**
 * @brief Get acknowledgment from server.
 * @param sockfd Socket file descriptor.
 * @return ack value if successful, -1 otherwise.
*/
int getack(int sockfd);


/**
 * @brief Send data message to server.
 * @param sockfd Socket file descriptor.
 * @param msg Message string.
 * @return 1 if successful, 0 otherwise.
*/
int sndmsg(int sockfd, const char *msg);


/**
 * @brief Send signal to server.
 * @param sockfd Socket file descriptor.
 * @param signo Signal number.
 * @return 1 if successful, 0 otherwise.
*/
int sndsig(int sockfd, char signo);


/**
 * @brief Print program usage info.
 * @param prog Program name.
*/
void usage(const char *prog);


/**
 * @brief rls initial checks.
 * @return 1 if all checks are passed, 0 otherwise
*/
int rls_checks(void);


/**
 * @brief Get configuration value from configuration file.
 * @param key Configuration key.
 * @param value Configuration value buffer.
 * @param n Size of the value buffer.
 * @return 1 if successful, 0 otherwise.
*/
int config_get(char *key, char *value, size_t n);



#endif /* _DEFINES_H */
