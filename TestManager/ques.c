#include "TM.h"

void handleGetQuestion(Students *currStudent) {
    // Create custom filename
    char filename[BUFFERSIZE] = "";
    strcat(filename, currStudent->username);
    strcat(filename, currStudent->password);
    strcat(filename, ".csv");

    // Get question file from QB
    handleQBget(filename);
}

int handleDisplayQuestion(int socket, char *buffer, Students *currStudent) {
    // Create custom filename
    char filename[BUFFERSIZE] = "";
    strcat(filename, currStudent->username);
    strcat(filename, currStudent->password);
    strcat(filename, ".csv");

    // If student file already exists
    int quesIdx;
    if (access(filename, F_OK) == 0) {
        quesIdx = currStudent->quesIdx;
        storeStudentQuestions(filename, currStudent);
    } else { // If student file does not exists, create one
        currStudent->quesIdx = 0;
        currStudent->grade = 0;
        quesIdx = currStudent->quesIdx;
        handleGetQuestion(currStudent);
        // Store student's allocated questions
        storeStudentQuestions(filename, currStudent);
    }

    // Display questions
    char *quesHTML = {0};
    quesHTML = getQuestionHTML(quesHTML, currStudent);
    sendResponse(socket, quesHTML);
    free(quesHTML);

    // Get the answer inputted by user
    int isCorrect = 0;
    if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        
        char encoded_ans[BUFFERSIZE];
        char decoded_ans[BUFFERSIZE];

        if (strstr(buffer, "mcqpy=") != NULL) {                         // MCQPY
            sscanf(strstr(buffer, "mcqpy="), "mcqpy=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("mcqpy", currStudent->allocated[quesIdx].question, decoded_ans);  // If wrong, minus mark by 1
        } else if (strstr(buffer, "mcqc=") != NULL) {                   // MCQC
            sscanf(strstr(buffer, "mcqc="), "mcqc=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("mcqc", currStudent->allocated[quesIdx].question, decoded_ans);   // If wrong, minus mark by 1
        } else if (strstr(buffer, "pcqpy=") != NULL) {                  // PCQPY
            sscanf(strstr(buffer, "pcqpy="), "pcqpy=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("pcqpy", currStudent->allocated[quesIdx].question, decoded_ans);  // If wrong, minus mark by 1
        } else if (strstr(buffer, "pcqc=") != NULL) {                   // PCQC
            sscanf(strstr(buffer, "pcqc="), "pcqc=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("pcqc", currStudent->allocated[quesIdx].question, decoded_ans);   // If wrong, minus mark by 1
        }

        currStudent->allocated[quesIdx].isCorrect = isCorrect;
        printf("isCorrect %d\n", isCorrect);
        printf("numAttempts before %d\n", currStudent->allocated[quesIdx].numAttempts);
        if (isCorrect == 1 || currStudent->allocated[quesIdx].numAttempts == 0){
            currStudent->grade += currStudent->allocated[quesIdx].numAttempts;
            currStudent->quesIdx++; // when no next button
            char *answerHTML = {0};
            answerHTML = getAnswerHTML(answerHTML, currStudent, isCorrect, decoded_ans);
            sendResponse(socket, answerHTML);
            free(answerHTML);
        } 
        else if (isCorrect == 0) {
            currStudent->allocated[quesIdx].numAttempts--;
        }
        printf("numAttempts after %d\n", currStudent->allocated[quesIdx].numAttempts);
    }
    return isCorrect;
}

void urlDecode(char *str, char *out) {
    unsigned int c;
    while (*str) {
        if (*str == '%' && isxdigit(*(str + 1)) && isxdigit(*(str + 2))) {
            sscanf(str + 1, "%2x", &c);
            *out++ = (char) c;
            str += 3;
        } else if (*str == '+') {
            *out++ = ' ';
            str++;
        } else {
            *out++ = *str++;
        }
    }
    *out = '\0';
}