#include "TM.h"

int handleDisplayQuestion(int socket, int quesIdx, char *buffer, char *username, char *password) {
    // Create custom filename
    char filename[100] = "";
    strcat(filename, username);
    strcat(filename, password);
    strcat(filename, ".csv");

    // Store questions in Question struct
    storeQuestions(filename);

    // Display questions
    char *quesHTML = {0};
    quesHTML = getQuestionHTML(quesIdx, quesHTML);
    sendResponse(socket, quesHTML);
    // Get the answer inputted by user
    if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        char encoded_ans[BUFFERSIZE];
        char decoded_ans[BUFFERSIZE];
        // If it is a MCQ
        if (strstr(buffer, "mcq=") != NULL) {
            sscanf(strstr(buffer, "mcq="), "mcq=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            // TODO : send decoded_ans to QB for checking
            if (QBcheckMCQAnswer(decoded_ans)) {

            }
        }
        // If it is a PCQ
        if (strstr(buffer, "pcq=") != NULL) {
            sscanf(strstr(buffer, "pcq="), "pcq=%s", encoded_ans);
            urlDecode(encoded_ans, decoded_ans); 
            // TODO : send decoded_ans to QB for checking
            if (QBcheckPCQAnswer(decoded_ans)) {
                
            }
        }
    }
    free(quesHTML);
    return 3;   // tbc
}

void storeQuestions(char *filename) {
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
            strncpy(questions[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            questions[quesIdx].isMCQ = 0;
            quesIdx++;
        } else if (strcmp(type, "mcqpy") == 0 || strcmp(type, "mcqc") == 0) {
            strncpy(questions[quesIdx].question, ques, MAX_QUESTION_LENGTH);
            questions[quesIdx].isMCQ = 1;
            // Store options
            for (int j = 0; j < MAX_OPTIONS; j++) {
                token = strtok(NULL, ",");
                strcpy(questions[quesIdx].options[j], token);
            }
            quesIdx++;
        } else {
            perror("[-] Invalid question type.\n");
            continue;
        }
    }
    fclose(fp);
}

char* getQuestionHTML(int idx, char *quesHTML) {
    quesHTML = (char*) realloc(quesHTML, BUFFERSIZE);
    if (questions[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><p>%s</p><form method=\"post\">\
                        <input type=\"radio\" id=\"a\" name=\"mcq\" value=\"%s\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"b\" name=\"mcq\" value=\"%s\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"c\" name=\"mcq\" value=\"%s\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"d\" name=\"mcq\" value=\"%s\"><label>%s</label><br><br>\
                        <button type=\"submit\">Submit</button>\
                        </form></body></html>", idx+1, MAX_QUESTIONS, questions[idx].question,\
                        questions[idx].options[0], questions[idx].options[0], questions[idx].options[1], questions[idx].options[1],\
                        questions[idx].options[2], questions[idx].options[2], questions[idx].options[3], questions[idx].options[3]);
    } else {
        sprintf(quesHTML, "<html><body><h1>Question %d/%d</h1><label for=\"pcq\">%s</label><form method=\"post\"><br>\
                        <textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br><br>\
                        <button type=\"submit\">Submit</button>\
                        </form></body></html>", idx+1, MAX_QUESTIONS, questions[idx].question);
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

void handleAnswersToQB(char *buffer) {

}