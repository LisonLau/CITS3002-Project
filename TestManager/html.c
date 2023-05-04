#include "TM.h"

char* getQuestionHTML(char *quesHTML, Students *currStudent) {
    int idx = currStudent->quesIdx;
    quesHTML = (char*) realloc(quesHTML, HTMLSIZE);
    char *backButton = "<input type=\"button\" name=\'back\' value=\"Back\" onclick=\"history.back()\">";
    char *nextButton = "<form method=\'post\'><input type=\'submit\' name=\'next\' value=\'Next\'></form>";
    char *logoutButton = "<form method=\'post\'><input type=\"submit\" name=\'logout\' value=\"Logout\"></form>";
    if (currStudent->allocated[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body><form method=\"post\">%s   \
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><p>%s</p><form method=\"post\">               \
                        <input type=\"radio\" id=\"a\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"b\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"c\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <input type=\"radio\" id=\"d\" name=\"%s\" value=\"%s\"><label>%s</label><br>                   \
                        <br>%s<button type=\"submit\">Submit</button>%s</form></body></html>",                          \
                        logoutButton, idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].question,    \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[0], currStudent->allocated[idx].options[0], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[1], currStudent->allocated[idx].options[1], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[2], currStudent->allocated[idx].options[2], \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].options[3], currStudent->allocated[idx].options[3], \
                        idx > 0 ? backButton : "", idx < MAX_QUESTIONS-1 ? nextButton : "");
    } else { // PCQ
        sprintf(quesHTML, "<html><body><form method=\"post\">%s\
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label>               \
                        <form method=\"post\"><br><textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br>     \
                        <br>%s<button type=\"submit\">Submit</button>%s</form></body></html>",                        \
                        logoutButton, idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,              \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].question, \
                        idx > 0 ? backButton : "", idx < MAX_QUESTIONS-1 ? nextButton : "");
    }
    return quesHTML;
}

char* getFinishHTML(int socket, char *buffer, int grade, char *finishHTML) {
    finishHTML = (char*) realloc(finishHTML, BUFFERSIZE);
    char *logoutButton = "<form method=\'post\'><input type=\"submit\" name=\'logout\' value=\"Logout\"></form>";
    sprintf(finishHTML, "<html><body>%s<h1>Test Finished</h1>       \
                        <p>Your grade is: %d/%d</p></body></html>", \
                        logoutButton, grade, MAX_QUESTIONS*3);
    return finishHTML;
}

char* getAnswerHTML(char *answerHTML, Students *currStudent, int isCorrect, char *stuAnswer) {
    int idx = currStudent->quesIdx;
    answerHTML = (char*) realloc(answerHTML, BUFSIZ);
    char *logoutButton = "<form method=\'post\'><input type=\"submit\" name=\'logout\' value=\"Logout\"></form>";
    char *backButton = "<input type=\"button\" name=\'back\' value=\"Back\" onclick=\"history.back()\">";
    char *nextButton = "<form method=\'post\'><input type=\'submit\' name=\'next\' value=\'Next\'></form>";
    char *correctMessage = "<p>Your answer is correct!</p>";
    char *wrongMessage   = "<p>Your answer is wrong!</p>";
    sprintf(answerHTML, "<html><body>%s<h1>Question %d/%d</h1><p>Your grade is: %d/%d</p> \
                        <p>%s</p><p>Your answer is: %s</p>%s<p>Correct answer is: %s</p>  \
                        %s%s</body></html>", \
                        logoutButton, idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,\
                        currStudent->allocated[idx].question, stuAnswer, isCorrect ? correctMessage : wrongMessage,\
                        "correct answer", idx > 0 ? backButton : "", nextButton);
    return answerHTML;
}