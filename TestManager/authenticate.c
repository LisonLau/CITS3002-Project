#include "TM.h"

int authenticate(char *username, char *password) {
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(username, students[i].username) == 0 && strcmp(password, students[i].password) == 0) {
            return 1; 
        }
    }
    return 0;
}