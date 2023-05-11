#include "TM.h"

// HTML variables
char *backButton   = "<form method=\'post\'><input type=\'submit\' name=\'back\' value=\'Back\'></form>";
char *nextButton   = "<form method=\'post\'><input type=\'submit\' name=\'next\' value=\'Next\'></form>";
char *logoutButton = "<form method=\'post\'><input type=\"submit\" name=\'logout\' value=\"Logout\"></form>";
char *failMessage  = "<p>Login failed. Try again.</p><br>";
char *wrongAnswer  = "<p>Your answer is wrong. Try again.</p>";
char *correctMessage = "<p>Your answer is correct!</p>";
char *wrongMessage   = "<p>Your answer is wrong!</p>";
char *blueBG = "style=\"background-color:#BBECF6\"";
char *correctColor = "style=\"background-color:#C6F099\"";
char *wrongColor = "style=\"background-color:#F6B0AD\"";
char *idleColor = "style=\"background-color:#F0F29F\"";

char* getQuestionHTML(char *quesHTML, Students *currStudent, int index) {
    int idx = currQuestion[index];
    int isCorrect = currStudent->allocated[idx].isCorrect; //(isCorrect ? correctColor : wrongColor)
    // int isDone = currStudent->allocated[idx].isDone;
    quesHTML = (char*) realloc(quesHTML, HTMLSIZE);
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
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label>       \
                        <form method=\"post\"><br><textarea name=\"%s\" rows=\"20\" cols=\"60\"></textarea> \
                        <br><br><button type=\"submit\">Submit</button>%s</form></body></html>",             \
                        blueBG, logoutButton, (idx > 0) ? backButton : "", (idx < MAX_QUESTIONS-1) ? nextButton : "",\
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,                           \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].question,              \
                        currStudent->allocated[idx].type,                                                    \
                        (currStudent->allocated[idx].isCorrect || currStudent->allocated[idx].numAttempts == 3) ? "" : wrongAnswer);
    }
    return quesHTML;
}

char* getFinishHTML(int socket, char *buffer, int grade, char *finishHTML, int index) {
    finishHTML = (char*) realloc(finishHTML, BUFSIZ);
    sprintf(finishHTML, "<html><body %s>%s<h1>Test Finished</h1>    \
                        <p>Your grade is: %d/%d</p></body></html>", \
                        blueBG, logoutButton, grade, MAX_QUESTIONS*3);
    return finishHTML;
}

char* getAnswerHTML(char *answerHTML, Students *currStudent, char *correctAns, int index) {
    int idx = currQuestion[index];
    int isCorrect = !strcmp(currStudent->allocated[idx].finalStuAns, correctAns);
    
    answerHTML = (char*) realloc(answerHTML, BUFSIZ);
    sprintf(answerHTML, "<html><body %s>%s%s%s<h1 %s>Question %d/%d</h1><p>Your grade is: %d/%d</p>     \
                        <p>%s</p><p>Your answer is: %s</p>%s<p>Correct answer is: %s</p></body></html>",\
                        blueBG, logoutButton, (idx > 0) ? backButton : "", nextButton, isCorrect ? correctColor : wrongColor, \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].question,      \
                        currStudent->allocated[idx].finalStuAns, isCorrect ? correctMessage : wrongMessage, correctAns);
    return answerHTML;
}

char* getLoginHTML(char *loginHTML, int failed) {
    loginHTML = (char*) realloc(loginHTML, BUFSIZ);
    sprintf(loginHTML, "<html><body %s><h1>Login</h1><form method=\"post\">\
                        <label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br>\
                        <label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br>\
                        %s<button type=\"submit\">Login</button></form></body></html>", blueBG, failed ? failMessage : "");
    return loginHTML;
}