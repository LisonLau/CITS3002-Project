// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_QUESTION_LENGTH 500
#define MAX_STUDENTS        100
#define MAX_QUESTIONS       10
#define MAX_OPTIONS         4
#define MAX_OPTION_LENGTH   100

#define BUFFERSIZE          1024

typedef struct Questions {
    char type[10];  
    char question[MAX_QUESTION_LENGTH];
    int  isMCQ;  // 1 means MCQ, 0 means PCQ
    char options[MAX_OPTIONS][MAX_OPTION_LENGTH];
    char finalStuAns[BUFFERSIZE];
    int  numAttempts;
    int  isCorrect;
    int  isDone; // 1 means question is done, 0 means not done
} Questions; 

typedef struct Students {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char filename[BUFFERSIZE];
    char ipAddress[255];
    int  hasQuesFile;
    int  loggedIn;
    int  grade;
    Questions allocated[MAX_QUESTIONS];
} Students;

typedef struct Result {
    int  isCorrect;
    char studentAns[BUFFERSIZE];
    char studentOutput[BUFFERSIZE];
} Result;

// Global variables
Students    students[MAX_STUDENTS];
int         currQuestion[MAX_STUDENTS]; // records current question the student is at
int         numStudents;
int         TMserver; // server socket file descriptor
const char  *SERVER_HOST;
const char  *CLIENT_HOST;