#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#define SERVER_PORT         8080
#define CLIENT_PORT         8888
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_QUESTION_LENGTH 500
#define MAX_STUDENTS        100
#define MAX_QUESTIONS       10
#define MAX_OPTIONS         4
#define MAX_OPTION_LENGTH   100
#define MAX_CLIENTS         30
#define BUFFERSIZE          1024
#define HTMLSIZE            16384

typedef struct Questions {
    char type[10];
    char question[MAX_QUESTION_LENGTH];
    int  isMCQ;  // 1 means MCQ, 0 means PCQ
    char options[MAX_OPTIONS][MAX_OPTION_LENGTH];
    int  numAttempts;
    int  isCorrect;
    int  isDone; // 1 means question is done, 0 means not done
} Questions; 

typedef struct Students {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char ipAddress[255];
    int  loggedIn;
    int  quesIdx;
    int  grade;
    Questions allocated[MAX_QUESTIONS];
} Students;

typedef struct Result {
    int  isCorrect;
    char stuAnswer[BUFFERSIZE];
} Result;

Questions   questions[MAX_QUESTIONS];  
Students    students[MAX_STUDENTS];
int         numStudents;
char        *HOST;

// user.c
extern void   storeUsers();
extern int    authenticateUsers(char *, char *);
extern void   storeStudentQuestions(char *, Students *);
// TM_web.c
extern void   runTMforWeb();
extern int    checkLoggedIn(char *, int);
extern int    handleUserLogin(int, char *, char *);
extern void   sendResponse(int, char *);
// ques.c
extern int    handleDisplayTest(int, char *, Students *);
extern Result handleUserAnswers(char *, Students *);
extern void   handleMarkAttempts(int, Result, Students *);
extern void   handleDisplayQuestion(int, char *, Students *);
extern void   urlDecode(char *, char *);
// TM_QB.c
extern int    handleQBcheck(char *, char *, char *);
extern int    sendQBCheck(int, char *, char *, char *);
extern void   handleQBget(char *);
extern void   sendQBget(int, char *);
// html.c
extern char*  getQuestionHTML(char *, Students *);
extern char*  getFinishHTML(int, char *, int, char *);
extern char*  getAnswerHTML(char *, Students *, int, char *);