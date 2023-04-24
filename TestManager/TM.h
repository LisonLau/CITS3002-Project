#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT                8080
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_STUDENTS        100
#define BUFFERSIZE          1024

typedef struct Student {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Student;

Student     students[MAX_STUDENTS];
int         numStudents;

int server_fd, new_socket, valread;
struct sockaddr_in address;

extern void storeUsers();
extern int  authenticateUsers(char *, char *);
extern void getUserLogin();
extern void runTmServer();
// extern void handle_socket(int, int, int, struct sockaddr_in *, int, char *, char *, char *, char *);