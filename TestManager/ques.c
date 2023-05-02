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
    int     isCorrect = 0;
    int     quesIdx;
    char    *username = currStudent->username;
    char    *password = currStudent->password;
    
    // Create custom filename
    char filename[BUFFERSIZE] = "";
    strcat(filename, username);
    strcat(filename, password);
    strcat(filename, ".csv");

    // If student file already exists
    if (access(filename, F_OK) == 0) {
        quesIdx = currStudent->quesIdx;
        storeStudentQuestions(filename, currStudent);
    } else { // If student file does not exists, create one
        currStudent->quesIdx = 0;
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
    }
    return isCorrect;
}

char* getQuestionHTML(char *quesHTML, Students *currStudent) {
    int idx = currStudent->quesIdx;
    quesHTML = (char*) realloc(quesHTML, BUFSIZ);
    char *backButton = "<input type=\"button\" value=\"Back\" onclick=\"history.back()\">";
    if (currStudent->allocated[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><p>%s</p><form method=\"post\">\
                        <input type=\"radio\" id=\"a\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"b\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"c\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"d\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <br>%s<button type=\"submit\">Submit</button></form></body></html>",                            \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].question,                  \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[0], currStudent->allocated[idx].options[0], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[1], currStudent->allocated[idx].options[1], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[2], currStudent->allocated[idx].options[2], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[3], currStudent->allocated[idx].options[3], \
                        idx > 0 ? backButton : "");
    } else { // PCQ
        sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label>   \
                        <form method=\"post\"><br><textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br>        \
                        <br>%s<input type=\"submit\" value=\"Submit\"></form></body></html>",                           \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,              \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].question, \
                        idx > 0 ? backButton : "");
    }
    return quesHTML;
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