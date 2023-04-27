#include "TM.h"

void handleDisplayQuestion(int socket, char *username, char *password) {
    // Create custom filename
    char filename[100] = "";
    strcat(filename, username);
    strcat(filename, password);
    strcat(filename, ".csv");

    // Store questions in Question struct
    storeQuestions(filename);

    // Display questions
    for (int i = 0; i < 1; i++) {
        char *quesHTML = getQuestionHTML(i);
        sendResponse(socket, quesHTML);
    }
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

char* getQuestionHTML(int idx) {
    char* quesHTML = (char*) malloc(BUFFERSIZE);
    if (questions[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body><h1>Question %d</h1><p>%s</p><form>\
                        <input type=\"radio\" id=\"a\" name=\"mcq\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"b\" name=\"mcq\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"c\" name=\"mcq\"><label>%s</label><br>\
                        <input type=\"radio\" id=\"d\" name=\"mcq\"><label>%s</label><br>\
                        <button type=\"submit\">Submit</button>\
                        </form></body></html>", idx+1, questions[idx].question,\
                        questions[idx].options[0], questions[idx].options[1], questions[idx].options[2], questions[idx].options[3]);
    } else {
        sprintf(quesHTML, "<html><body><h1>Question %d</h1><label for=\"pcq\">%s</label><form><br>\
                        <textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br><br>\
                        <button type=\"submit\">Submit</button>\
                        </form></body></html>", idx+1, questions[idx].question);
    }
    return quesHTML;
}