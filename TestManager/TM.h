#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_STUDENTS        100

typedef struct Student {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Student;

Student     students[MAX_STUDENTS];
int numStudents;

extern void storeRegistered();
extern int authenticate(char *, char *);
extern void getUserLogin();