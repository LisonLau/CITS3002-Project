#include "TM.h"

void handleDisplayTest(int socket, char *buffer, Students *currStudent, int index) {
    // Create custom filename
    char filename[BUFFERSIZE] = "";
    strcat(filename, currStudent->username);
    strcat(filename, currStudent->password);
    strcat(filename, ".csv");

    // If student file does not exists, create one
    if (access(filename, F_OK) != 0) {
        handleQBgetFile(filename);
        //currStudent->quesIdx = 0;
        currStudent->grade = 0;
    } 

    // Store student's allocated questions
    storeStudentQuestions(filename, currStudent); // TODO ask lianne about &

    // Handle the test
    Result result;
    if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        // Handle user answers WHEN submit
        result = handleUserAnswers(buffer, currStudent, index);

        // Handle grading and number of attempts
        currStudent->allocated[currQuestion[index]].isCorrect = result.isCorrect;
        handleMarkAttempts(socket, result, currStudent, index, buffer);

        // Handle display questions WHEN nothing to do
        handleDisplayQuestion(socket, buffer, currStudent, index);
    }
}

Result handleUserAnswers(char *buffer, Students *currStudent, int index) {
    // Get the answer inputted by user on SUBMIT button press
    Result result;
    result.isCorrect = 0;
    char encoded_ans[BUFFERSIZE];
    
    if (strstr(buffer, "mcqpy=") != NULL) {                         // MCQPY
        sscanf(strstr(buffer, "mcqpy="), "mcqpy=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("mcqpy", currStudent->allocated[currQuestion[index]].question, result.studentAns);  // If wrong, minus mark by 1
    } else if (strstr(buffer, "mcqc=") != NULL) {                   // MCQC
        sscanf(strstr(buffer, "mcqc="), "mcqc=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("mcqc", currStudent->allocated[currQuestion[index]].question, result.studentAns);   // If wrong, minus mark by 1
    } else if (strstr(buffer, "pcqpy=") != NULL) {                  // PCQPY
        sscanf(strstr(buffer, "pcqpy="), "pcqpy=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("pcqpy", currStudent->allocated[currQuestion[index]].question, result.studentAns);  // If wrong, minus mark by 1
    } else if (strstr(buffer, "pcqc=") != NULL) {                   // PCQC
        sscanf(strstr(buffer, "pcqc="), "pcqc=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("pcqc", currStudent->allocated[currQuestion[index]].question, result.studentAns);   // If wrong, minus mark by 1
    }
    return result;
}

void handleMarkAttempts(int socket, Result result, Students *currStudent, int index, char *buffer) {
    int numAttempts = currStudent->allocated[currQuestion[index]].numAttempts;
    int isCorrect = currStudent->allocated[currQuestion[index]].isCorrect;
    //int isDone = currStudent->allocated[currQuestion[index]].isDone;
    
    printf("isCorrect %d\n", currStudent->allocated[currQuestion[index]].isCorrect);
    printf("numAttempts before %d\n", currStudent->allocated[currQuestion[index]].numAttempts);
    
    // If question is correct OR 3 attempts made
    /*
    if (isCorrect || numAttempts == 0){
        currStudent->allocated[currQuestion[index]].isDone = 1;
        currStudent->grade += numAttempts;
        char *answerHTML = {0};
        char *correctAns = handleQBgetAns(currStudent->allocated[currQuestion[index]].type, currStudent->allocated[currQuestion[index]].question);
        answerHTML = getAnswerHTML(answerHTML, currStudent, isCorrect, result.studentAns, correctAns, index);
        sendResponse(socket, answerHTML);
        free(answerHTML);
        free(correctAns);
    } else if (isCorrect == 0 &&\
     (strstr(buffer, "POST / HTTP/1.1") && strstr(buffer, "back=") == 0 \
     && strstr(buffer, "next=") == 0 && strstr(buffer, "logout=") == 0)) { // If question is wrong, decrement number of attempts
        currStudent->allocated[currQuestion[index]].numAttempts--;
    } else if (isDone) {
        char *answerHTML = {0};
        char *correctAns = handleQBgetAns(currStudent->allocated[currQuestion[index]].type, currStudent->allocated[currQuestion[index]].question);
        answerHTML = getAnswerHTML(answerHTML, currStudent, isCorrect, result.studentAns, correctAns, index);
        sendResponse(socket, answerHTML);
        free(answerHTML);
        free(correctAns);
    }*/

    if (strstr(buffer, "POST / HTTP/1.1") && strstr(buffer, "back=") == 0 \
        && strstr(buffer, "next=") == 0 && strstr(buffer, "logout=") == 0) {
            if (isCorrect || numAttempts == 0) {
                currStudent->allocated[currQuestion[index]].isDone = 1;
                if (currQuestion[index] == 0 && numAttempts > 0) 
                    currStudent->grade += numAttempts + 1;
                else
                    currStudent->grade += numAttempts;
                char *answerHTML = {0};
                char *correctAns = handleQBgetAns(currStudent->allocated[currQuestion[index]].type, currStudent->allocated[currQuestion[index]].question);
                answerHTML = getAnswerHTML(answerHTML, currStudent, isCorrect, result.studentAns, correctAns, index);
                sendResponse(socket, answerHTML);
                free(answerHTML);
                free(correctAns);
            }
            currStudent->allocated[currQuestion[index]].numAttempts--;
    }
    printf("numAttempts after %d\n", currStudent->allocated[currQuestion[index]].numAttempts);
}

void handleDisplayQuestion(int socket, char *buffer, Students *currStudent, int index) {
    // Display the question if student is not done with the question
    if (currStudent->allocated[currQuestion[index]].isDone == 0) {
        char *quesHTML = {0};
        quesHTML = getQuestionHTML(quesHTML, currStudent, index);
        sendResponse(socket, quesHTML);
        free(quesHTML);
    }
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
