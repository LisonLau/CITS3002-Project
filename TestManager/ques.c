// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "TM.h"

/**
 * @brief Handles displaying the test on a web browser
 * @param socket the socket file descriptor
 * @param HTTPrequest the HTTP request received from client web browser
 * @param currStudent current student information
 * @param index index of student's current question
 */
void handleDisplayTest(int socket, char *HTTPrequest, Students *currStudent, int index) {
    // Create custom filename


    // If student file does not exists, create one
    if (access(currStudent->filename, F_OK) != 0) {
        handleQBgetFile(currStudent->filename);
        currStudent->grade = 0;
        currStudent->hasQuesFile = 1;
        // Store student's allocated questions
        storeStudentQuestions(currStudent->filename, currStudent);
    }

    // Handle the test
    Result result;
    if (strstr(HTTPrequest, "POST / HTTP/1.1") != NULL) {
        // Handle user answers WHEN submit
        result = handleUserAnswers(HTTPrequest, currStudent, index);

        // Handle grading and number of attempts
        if (!currStudent->allocated[currQuestion[index]].isCorrect) {
            currStudent->allocated[currQuestion[index]].isCorrect = result.isCorrect;
        }
        handleMarkAttempts(socket, HTTPrequest, currStudent, index, result);

        // Handle display answer page AFTER question is done
        handleDisplayAnswer(socket, currStudent, index);

        // Handle display questions WHEN nothing to do
        handleDisplayQuestion(socket, HTTPrequest, currStudent, index);
    }
}

/**
 * @brief Handles connecting to QB to deal with grading
 * @param HTTPrequest the HTTP request received from client web browser
 * @param currStudent current student information
 * @param index index of student's current question
 * @return Result contains the student answer and whether it is correct
 */
Result handleUserAnswers(char *HTTPrequest, Students *currStudent, int index) {
    // Get the answer inputted by user on SUBMIT button press
    Result result;
    result.isCorrect = 0;
    char encoded_ans[BUFFERSIZE];
    
    if (strstr(HTTPrequest, "mcqpy=") != NULL) {        // MCQPY
        sscanf(strstr(HTTPrequest, "mcqpy="), "mcqpy=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("mcqpy", currStudent->allocated[currQuestion[index]].question, result.studentAns);  // If wrong, minus mark by 1
    } else if (strstr(HTTPrequest, "mcqc=") != NULL) {  // MCQC
        sscanf(strstr(HTTPrequest, "mcqc="), "mcqc=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("mcqc", currStudent->allocated[currQuestion[index]].question, result.studentAns);   // If wrong, minus mark by 1
    } else if (strstr(HTTPrequest, "pcqpy=") != NULL) { // PCQPY
        sscanf(strstr(HTTPrequest, "pcqpy="), "pcqpy=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        result.isCorrect = handleQBcheck("pcqpy", currStudent->allocated[currQuestion[index]].question, result.studentAns);  // If wrong, minus mark by 1
    } else if (strstr(HTTPrequest, "pcqc=") != NULL) {  // PCQC
        sscanf(strstr(HTTPrequest, "pcqc="), "pcqc=%s", encoded_ans);
        urlDecode(encoded_ans, result.studentAns); 
        printf("%s\n", result.studentAns);
        result.isCorrect = handleQBcheck("pcqc", currStudent->allocated[currQuestion[index]].question, result.studentAns);   // If wrong, minus mark by 1
        printf("%s\n", result.studentAns);
    }
    return result;
}

/**
 * @brief Handles keeping track of number of attempts and student's marks
 * @param socket the socket file descriptor
 * @param HTTPrequest the HTTP request received from client web browser
 * @param currStudent current student information
 * @param index index of student's current question
 * @param result contains the student answer and whether it is correct
 */
void handleMarkAttempts(int socket, char *HTTPrequest, Students *currStudent, int index, Result result) {
    int numAttempts = currStudent->allocated[currQuestion[index]].numAttempts;
    int isCorrect = result.isCorrect;

    // If question is correct OR 3 attempts made 
    if (strstr(HTTPrequest, "mcq") || strstr(HTTPrequest, "pcq")) {
        currStudent->allocated[currQuestion[index]].numAttempts--;
        if (isCorrect || numAttempts == 1) {
            currStudent->allocated[currQuestion[index]].isDone = 1;
            currStudent->allocated[currQuestion[index]].isCorrect = result.isCorrect;
            strcpy(currStudent->allocated[currQuestion[index]].finalStuAns, result.studentAns);
            if (isCorrect)
                currStudent->grade += numAttempts;
            else if (numAttempts == 1)
                currStudent->grade += numAttempts-1;
        }
    }
}

/**
 * @brief Handles displaying the answer page on the web browser
 * @param socket the socket file descriptor
 * @param currStudent current student information
 * @param index index of student's current question
 */
void handleDisplayAnswer(int socket, Students *currStudent, int index) {
    // Display answer page when question is already done
    if (currStudent->allocated[currQuestion[index]].isDone) {
        char *answerHTML = {0};
        char *correctAns = {0};
        correctAns = handleQBgetAns(currStudent->allocated[currQuestion[index]].type, currStudent->allocated[currQuestion[index]].question);
        answerHTML = getAnswerHTML(answerHTML, currStudent, correctAns, index);
        sendHTMLpage(socket, answerHTML);
        if (answerHTML != NULL) {
            free(answerHTML);
            answerHTML = NULL;
        }
        if (correctAns != NULL) {
            free(correctAns);
            correctAns = NULL;
        }
    }
}

/**
 * @brief Handles displayed the question page on the web browser
 * @param socket the socket file descriptor
 * @param HTTPrequest the HTTP request received from client web browser
 * @param currStudent current student information
 * @param index index of student's current question
 */
void handleDisplayQuestion(int socket, char *HTTPrequest, Students *currStudent, int index) {
    // Display the question if student is not done with the question
    if (currStudent->allocated[currQuestion[index]].isDone == 0) {
        char *quesHTML = {0};
        quesHTML = getQuestionHTML(quesHTML, currStudent, index);
        sendHTMLpage(socket, quesHTML);
        if (quesHTML != NULL) {
            free(quesHTML);
            quesHTML = NULL;
        }
    }
}

/**
 * @brief URL decoding function 
 * @param str string to be decoded
 * @param out the decoded string
 */
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
