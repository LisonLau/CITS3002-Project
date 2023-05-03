#include "TM.h"

char* getQuestionHTML(char *quesHTML, Students *currStudent) {
    int idx = currStudent->quesIdx;
    quesHTML = (char*) realloc(quesHTML, BUFSIZ);
    char *backButton = "<input type=\"button\" value=\"Back\" onclick=\"history.back()\">";
    // char *nextButton = "<input type=\"button\" value=\"Next\" method=\"post\">";
    if (currStudent->allocated[idx].isMCQ) { // MCQ
        sprintf(quesHTML, "<html><body><form method=\"post\"><button name=\"logout\" value=\"Logout\">Logout</button>   \
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><p>%s</p><form method=\"post\">               \
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
        sprintf(quesHTML, "<html><body><form method=\"post\"><button name=\"logout\" value=\"Logout\">Logout</button>\
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><label for=\"%s\">%s</label>               \
                        <form method=\"post\"><br><textarea name=\"pcq\" rows=\"20\" cols=\"60\"></textarea><br>     \
                        <br>%s<input type=\"submit\" value=\"Submit\"></form></body></html>",                        \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3,              \
                        currStudent->allocated[idx].type, currStudent->allocated[idx].question, \
                        idx > 0 ? backButton : "");
    }
    return quesHTML;
}

char* getFinishHTML(int socket, char *buffer, int grade, char *finishHTML) {
    finishHTML = (char*) realloc(finishHTML, BUFFERSIZE);
    sprintf(finishHTML, "<html><body><form method=\"post\"><button name=\"logout\" value=\"Logout\">Logout</button></form>\
                        <h1>Test Finished</h1><p>Your grade is: %d/%d</p></body></html>", grade, MAX_QUESTIONS*3);
    return finishHTML;
}

char* getAnswerHTML(char *answerHTML, Students *currStudent, int isCorrect, char *stuAnswer) {
    int idx = currStudent->quesIdx;
    answerHTML = (char*) realloc(answerHTML, BUFSIZ);
    char *correctMessage = "<p>Your answer is correct!</p>";
    char *wrongMessage   = "<p>Your answer is wrong!</p>";
    sprintf(answerHTML, "<html><body><form method=\"post\"><button name=\"logout\" value=\"Logout\">Logout</button>\
                        <h1>Question %d/%d</h1><p>Your grade is: %d/%d</p><               \
                        <p>%s</p<p>Your answer is: %s</p>%s<p>Correct answer is: %s</p>   \
                        <input type=\"button\" value=\"Back\" onclick=\"history.back()\"> \
                        <br><input type=\"button\" value=\"Next\">",                      \
                        idx+1, MAX_QUESTIONS, currStudent->grade, MAX_QUESTIONS*3, currStudent->allocated[idx].question,\
                        stuAnswer, isCorrect ? correctMessage : wrongMessage, "correct answer");
    return answerHTML;
}