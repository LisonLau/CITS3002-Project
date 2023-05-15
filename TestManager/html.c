// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "TM.h"
#include "html.h"

/**
 * @brief Updates quesHTML string with the question page HTML code
 * @param quesHTML the string containing the question page HTML code
 * @param currStudent current student information used to generate the question page HTML code
 * @param index the index of the student's current question
 * @return char* returns quesHTML string with the question page HTML code
 */
char* getQuestionHTML(char *quesHTML, Students *currStudent, int index) {
    int idx = currQuestion[index];
    int isCorrect = currStudent->allocated[idx].isCorrect;
    quesHTML = (char*) realloc(quesHTML, HTMLSIZE);
    if (quesHTML == NULL) {
        perror("[!] Error: Failed to allocate memory for quesHTML.\n");
        exit(EXIT_FAILURE);
    }
    if (currStudent->allocated[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body %s>%s%s%s<h1 %s>Question %d/%d</h1><p>Your grade is: %d/%d</p>   \
                        <p>Attempts left: %d</p><p>%s</p><form method=\"post\">                         \
                        <input type=\"radio\" id=\"a\" name=\"%s\" value=\"%s\"><label>%s</label><br>   \
                        <input type=\"radio\" id=\"b\" name=\"%s\" value=\"%s\"><label>%s</label><br>   \
                        <input type=\"radio\" id=\"c\" name=\"%s\" value=\"%s\"><label>%s</label><br>   \
                        <input type=\"radio\" id=\"d\" name=\"%s\" value=\"%s\"><label>%s</label><br>   \
                        <br><button type=\"submit\">Submit</button>%s</form></body></html>",            \
                        blueBG, logoutButton, idx > 0 ? backButton : "", idx < MAX_QUESTIONS-1 ? nextButton : "", (isCorrect == -1) ? idleColor : wrongColor, idx+1, MAX_QUESTIONS, \
                        currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].numAttempts, currStudent->allocated[idx].question, \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[0], currStudent->allocated[idx].options[0],   \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[1], currStudent->allocated[idx].options[1],   \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[2], currStudent->allocated[idx].options[2],   \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[3], currStudent->allocated[idx].options[3],   \
                        (currStudent->allocated[idx].isCorrect || currStudent->allocated[idx].numAttempts == 3) ? "" : wrongAnswer);
    } else { // PCQ
        sprintf(quesHTML, "<html><body %s>%s%s%s<form method=\"post\">\
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label>                  \
                        <form method=\"post\"><br><textarea name=\"pcq\" rows=\"20\" cols=\"60\" %s></textarea>         \
                        <br><br><button type=\"submit\">Submit</button>%s</form></body></html>",                        \
                        blueBG, logoutButton, (idx > 0) ? backButton : "", (idx < MAX_QUESTIONS-1) ? nextButton : "",   \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,                                      \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].question, onKeyDown,              \
                        (currStudent->allocated[idx].isCorrect || currStudent->allocated[idx].numAttempts == 3) ? "" : wrongAnswer);
    }
    return quesHTML;
}

/**
 * @brief Updates finishHTML string with the finish test page HTML code
 * @param finishHTML the string containing the finish test page HTML code
 * @param grade the student's final grade
 * @return char* returns finishHTML string with the finish test page HTML code
 */
char* getFinishHTML(char *finishHTML, int grade) {
    finishHTML = (char*) realloc(finishHTML, BUFSIZ);
    if (finishHTML == NULL) {
        perror("[!] Error: Failed to allocate memory for finishHTML.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(finishHTML, "<html><body %s>%s<h1>Test Finished</h1>    \
                        <p>Your grade is: %d/%d</p></body></html>", \
                        blueBG, logoutButton, grade, MAX_QUESTIONS*3);
    return finishHTML;
}

/**
 * @brief Updates answerHTML string with the finish question page HTML code
 * @param answerHTML the string containing the finish question page HTML code
 * @param currStudent current student information used to generate the finish question page HTML code
 * @param correctAns QB's correct answer to be displayed
 * @param index the index of the student's current question
 * @return char* returns answerHTML string with the finish question page HTML code
 */
char* getAnswerHTML(char *answerHTML, Students *currStudent, char *correctAns, int index) {
    int idx = currQuestion[index];
    int isCorrect = currStudent->allocated[idx].isCorrect;
    answerHTML = (char*) realloc(answerHTML, BUFSIZ);
    if (answerHTML == NULL) {
        perror("[!] Error: Failed to allocate memory for answerHTML.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(answerHTML, "<html><body %s>%s%s%s<h1 %s>Question %d/%d</h1><p>Your grade is: %d/%d</p>     \
                        <p>%s</p><p>Your answer is: %s</p>%s<p>Correct answer is: %s</p></body></html>",\
                        blueBG, logoutButton, (idx > 0) ? backButton : "", nextButton, isCorrect ? correctColor : wrongColor, \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].question,      \
                        currStudent->allocated[idx].finalStuAns, isCorrect ? correctMessage : wrongMessage, correctAns);
    return answerHTML;
}

/**
 * @brief Updates loginHTML string with the login page HTML code
 * @param loginHTML the string containing the login page HTML code
 * @param failed 1 is user failed to login, 0 otherwise
 * @return char* returns loginHTML string with the login page HTML code
 */
char* getLoginHTML(char *loginHTML, int failed) {
    loginHTML = (char*) realloc(loginHTML, BUFSIZ);
    if (loginHTML == NULL) {
        perror("[!] Error: Failed to allocate memory for loginHTML.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(loginHTML, "<html><body %s><h1>Login</h1><form method=\"post\">\
                        <label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br>\
                        <label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br>\
                        %s<button type=\"submit\">Login</button></form></body></html>", blueBG, failed ? failMessage : "");
    return loginHTML;
}
