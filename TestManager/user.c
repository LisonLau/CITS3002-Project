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
