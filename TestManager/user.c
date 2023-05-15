// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "TM.h"

/**
 * @brief store registered users in the student structure
 */
void storeUsers() {
    FILE *fp;
    char line[BUFFERSIZE];
    char *token;
    int count = 0;

    fp = fopen("students.csv", "r");
    if (fp == NULL) {
        perror("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
 
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Parse the line using strtok
        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(students[count].username, token, MAX_USERNAME_LENGTH - 1);
            students[count].username[MAX_USERNAME_LENGTH - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(students[count].password, token, MAX_PASSWORD_LENGTH - 1);
            students[count].password[strcspn(students[count].password, "\r\n")] = 0;
        }
        for (int i = 0; i < MAX_QUESTIONS; i++) {
            students[count].allocated[i].numAttempts = 3;
        }
        count++;
    }

    fclose(fp);
    numStudents = count;
}

/**
 * @brief checks and authenticates student login
 * @param username username from web browser input
 * @param password password from web browser input
 * @return 1 if student login succeeds, 0 if fails
 */
int authenticateUsers(char *username, char *password) {
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(username, students[i].username) == 0 && strcmp(password, students[i].password) == 0) {
            return 1; 
        }
    }
    return 0;
}

/**
 * @brief store the current student's questions into a structure
 * @param filename student's custom filename
 * @param currStudent current student information
 */
void storeStudentQuestions(char *filename, Students *currStudent) {
    FILE *fp;
    char line[BUFFERSIZE];
    char *token;
    int quesIdx = 0;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file.\n");
        exit(EXIT_FAILURE);
    }
    
    while (fgets(line, BUFFERSIZE, fp)) {
        char* type = strtok(line, ",");
        char* ques = strtok(NULL, ",");
        // Stops after reading 10 questions
        if (quesIdx == MAX_QUESTIONS) {
            continue;
        }
        
        currStudent->allocated[quesIdx].isCorrect = 0;
        // Check the question type and store the values accordingly
        if (strcmp(type, "pcqpy") == 0 || strcmp(type, "pcqc") == 0) {
            strncpy(currStudent->allocated[quesIdx].type, type, 10);
            strncpy(currStudent->allocated[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            currStudent->allocated[quesIdx].isMCQ = 0;
            quesIdx++;


        } else if (strcmp(type, "mcqpy") == 0 || strcmp(type, "mcqc") == 0) {
            strncpy(currStudent->allocated[quesIdx].type, type, 10);
            strncpy(currStudent->allocated[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            currStudent->allocated[quesIdx].isMCQ = 1;
            // Store options
            for (int j = 0; j < MAX_OPTIONS; j++) {
                token = strtok(NULL, ",");
                int len = strlen(token);
                if (token[len-1] == '\n') {
                    token[len-1] = '\0';
                }
                printf("'%s'\n", token);
                strcpy(currStudent->allocated[quesIdx].options[j], token);
            }
            quesIdx++;

        } else {
            perror("[-] Invalid question type.\n");
            continue;
        }
    }
    fclose(fp);
}
