// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "structs.h"

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
#include <signal.h>

#define MAX_CLIENTS         30

#define HTTPSIZE            4096
#define FILESIZE            8129
#define HTMLSIZE            16384   

#define SERVER_PORT         8080
#define CLIENT_PORT         8888
#define TEMP_IMG            "tempImg.png"

// main.c
extern void   clearMemory(int);

// user.c
extern void   storeUsers();
extern int    authenticateUsers(char *, char *);
extern void   storeStudentQuestions(char *, Students *);

// TM_web.c
extern void   runTMforWeb();
extern int    checkLoggedIn(char *, int);
extern int    handleUserLogin(int, char *, char *);
extern void   sendHTML(int, char *);
extern void   sendImage(int, char *);

// ques.c
extern void   handleDisplayTest(int, char *, Students *, int);
extern Result handleUserAnswers(char *, Students *, int);
extern void   handleMarkAttempts(char *, Students *, int, Result);
extern void   handleDisplayAnswer(int, Students *, int);
extern void   handleDisplayQuestion(int, Students *, int);
extern void   handleFinishTest(int, Students *);
extern void   urlDecode(char *, char *);

// TM_QB.c
extern int    createTMclient();
extern int    handleQBcheck(char *, char *, char *);
extern void   handleQBgetFile(char *);
extern char*  handleQBgetAns(char *, char *);
extern void   handleQBgetImg(char *, char *, char *);
extern void   socketSend(int, char *, char *);
extern void   receiveACK(int, char *, char *);

// html.c
extern char*  getQuestionHTML(char *, Students *, int);
extern char*  getFinishHTML(char *, int);
extern char*  getAnswerHTML(char *, Students *, char *, int);
extern char*  getLoginHTML(char *, int);
