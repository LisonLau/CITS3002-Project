#include "TM.h"

int QBcheckMCQAnswer(char *decoded_ans) {
    return 1;
}

int QBcheckPCQAnswer(char *decoded_ans) {
    return 1;
}

char* getFinishHTML(int socket, char *buffer, int grade, char *finishHTML) {
    finishHTML = (char*) realloc(finishHTML, BUFFERSIZE);
    sprintf(finishHTML, "<html><body><h1>Test Finished</h1>\
                    <p>Your grade is: %d/30</p></body></html>", grade);
    return finishHTML;
}