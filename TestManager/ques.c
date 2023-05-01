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

    // TODO: check if student file already exits
    if (access(filename, F_OK) == 0) {
        // file exists
        quesIdx = currStudent->quesIdx;
    } else {
        currStudent->quesIdx = 0;
        quesIdx = currStudent->quesIdx;
        // file doesn't exist...
        // Need to make one and get questions from QB 
    }

    // Store questions in Question struct
    storeQuestions(filename, currStudent);

    // Display questions
    char *quesHTML = {0};
    quesHTML = getQuestionHTML(quesIdx, quesHTML, currStudent);
    sendResponse(socket, quesHTML);

    // Get the answer inputted by user
    if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        
        char encoded_ans[BUFFERSIZE];
        char decoded_ans[BUFFERSIZE];
 
        if (strstr(buffer, "mcqpy=") != NULL) {                         // MCQPY
            sscanf(strstr(buffer, "mcqpy="), "mcqpy=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("mcqpy", currStudent->allocated_ques[quesIdx-1].question, decoded_ans);  // If wrong, minus mark by 1
        } else if (strstr(buffer, "mcqc=") != NULL) {                   // MCQC
            sscanf(strstr(buffer, "mcqc="), "mcqc=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("mcqc", currStudent->allocated_ques[quesIdx-1].question, decoded_ans);   // If wrong, minus mark by 1
        } else if (strstr(buffer, "pcqpy=") != NULL) {                  // PCQPY
            sscanf(strstr(buffer, "pcqpy="), "pcqpy=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("pcqpy", currStudent->allocated_ques[quesIdx-1].question, decoded_ans);  // If wrong, minus mark by 1
        } else if (strstr(buffer, "pcqc=") != NULL) {                   // PCQC
            sscanf(strstr(buffer, "pcqc="), "pcqc=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            isCorrect = handleQBcheck("pcqc", currStudent->allocated_ques[quesIdx-1].question, decoded_ans);   // If wrong, minus mark by 1
        }
    }
    currStudent->quesIdx++;
    free(quesHTML);
    return isCorrect;
}

void storeQuestions(char *filename, Students *currStudent) {
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
        // Check the question type and store the values accordingly
        if (strcmp(type, "pcqpy") == 0 || strcmp(type, "pcqc") == 0) {
            strncpy(currStudent->allocated_ques[quesIdx].type, type, 10);
            strncpy(currStudent->allocated_ques[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            currStudent->allocated_ques[quesIdx].isMCQ = 0;
            quesIdx++;
        } else if (strcmp(type, "mcqpy") == 0 || strcmp(type, "mcqc") == 0) {
            strncpy(currStudent->allocated_ques[quesIdx].type, type, 10);
            strncpy(currStudent->allocated_ques[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            currStudent->allocated_ques[quesIdx].isMCQ = 1;
            // Store options
            for (int j = 0; j < MAX_OPTIONS; j++) {
                token = strtok(NULL, ",");
                strcpy(currStudent->allocated_ques[quesIdx].options[j], token);
            }
            quesIdx++;
        } else {
            perror("[-] Invalid question type.\n");
            continue;
        }
    }
    fclose(fp);
}

char* getQuestionHTML(int idx, char *quesHTML, Students *currStudent) {
    quesHTML = (char*) realloc(quesHTML, BUFFERSIZE);
    if (currStudent->allocated_ques[idx].isMCQ) { // MCQ
        if (idx > 0) {  // If it's not the first question, display the Back button
            sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><p>%s</p><form method=\"post\">    \
                            <input type=\"radio\" id=\"a\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"b\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"c\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"d\" name=\"%s\" value=\"%s\"><label>%s</label><br><br>                   \
                            <input type=\"button\" value=\"Back\" onclick=\"history.back()\">                                   \
                            <input type=\"submit\" value=\"Submit\">                                                            \
                            </form></body></html>", idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated_ques[idx].question,         \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[0], currStudent->allocated_ques[idx].options[0], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[1], currStudent->allocated_ques[idx].options[1], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[2], currStudent->allocated_ques[idx].options[2], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[3], currStudent->allocated_ques[idx].options[3]);
        } else { // If it's first question
            sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><p>%s</p><form method=\"post\">    \
                            <input type=\"radio\" id=\"a\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"b\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"c\" name=\"%s\" value=\"%s\"><label>%s</label><br>                       \
                            <input type=\"radio\" id=\"d\" name=\"%s\" value=\"%s\"><label>%s</label><br><br>                   \
                            <input type=\"submit\" value=\"Submit\">                                                            \
                            </form></body></html>", idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated_ques[idx].question,         \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[0], currStudent->allocated_ques[idx].options[0], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[1], currStudent->allocated_ques[idx].options[1], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[2], currStudent->allocated_ques[idx].options[2], \
                            currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].options[3], currStudent->allocated_ques[idx].options[3]);
        }
    } else { // PCQ
        if (idx > 0) {
            sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label><form method=\"post\"><br>\
                            <textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br><br>\
                            <input type=\"button\" value=\"Back\" onclick=\"history.back()\">\
                            <input type=\"submit\" value=\"Submit\">\
                            </form></body></html>", idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].question);
        } else {
            sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label><form method=\"post\"><br>\
                            <textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br><br>\
                            <input type=\"submit\" value=\"Submit\">\
                            </form></body></html>", idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated_ques[idx].type, currStudent->allocated_ques[idx].question);
        }
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