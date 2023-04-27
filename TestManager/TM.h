#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT                8888
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_QUESTION_LENGTH 100
#define MAX_STUDENTS        100
#define MAX_QUESTIONS       10
#define MAX_OPTIONS         4
#define MAX_OPTION_LENGTH   100
#define BUFFERSIZE          1024

typedef struct Question {
    char question[MAX_QUESTION_LENGTH];
    int isMCQ;  // 1 means MCQ, 0 means PCQ
    char options[MAX_OPTIONS][MAX_OPTION_LENGTH];
} Question; 

typedef struct Student {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int grade;  // out of 10
    // Question allocated[MAX_QUESTIONS];
} Student;

Question    questions[MAX_QUESTIONS];  
Student     students[MAX_STUDENTS];
int         numStudents;

// user.c
extern void storeUsers();
extern int  authenticateUsers(char *, char *);
// TM_web.c
extern void runTMforWeb();
extern void handleUserLogin();
extern void sendResponse();
// ques.c
extern void handleDisplayQuestion();
extern void storeQuestions();
extern char* getQuestionHTML(int idx);