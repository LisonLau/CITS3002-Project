#include "TM.h"

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
        count++;
    }

    fclose(fp);
    numStudents = count;
}

int authenticateUsers(char *username, char *password) {
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(username, students[i].username) == 0 && strcmp(password, students[i].password) == 0) {
            return 1; 
        }
    }
    return 0;
}

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
        currStudent->allocated[quesIdx].numAttempts = 3;
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
