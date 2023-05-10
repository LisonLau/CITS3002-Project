#include "TM.h"

int createTMclient() {
    int TMclient;
    struct sockaddr_in QBaddress;

    // Create a client socket
    if ((TMclient = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in creating TM client socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] TM client socket created to get file.\n");

    // Set server address and port
    qb_addr.sin_addr.s_addr = inet_addr(clientIpAddr);
    qb_addr.sin_family      = AF_INET;
    qb_addr.sin_port        = htons(CLIENT_PORT);

    // Connect to server
    if (connect(TMclient, (struct sockaddr *)&QBaddress, sizeof(QBaddress)) < 0) {
        perror("[-] Error in connecting to QB.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection to QB successful.\n");

    return TMclient;
}

// Create TM client to send request from QB for question file
void handleQBgetFile(char *filename) {
    int TMclient = createTMclient();

    // Send the request for file message
    char message[BUFFERSIZE] = "";
    strcat(message, "get,");
    strcat(message, filename);
    socketSend(TMclient, message, "get file");

    // Receive the file from QB
    char filelines[BUFFERSIZE];
    int n;
    FILE *fp = fopen(filename, "wb"); 
    while ((n = recv(TMclient, filelines, BUFFERSIZE, 0)) > 0) {
        fwrite(filelines, sizeof(char), n, fp); 
    }
    fclose(fp);
    printf("[+] Question file '%s' received successfully.\n", filename);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");

    close(TMclient);
}

// Create TM client to send request from QB to check answer
int handleQBcheck(char *type, char *ques, char *ans) {
    int isCorrect = -1;
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    strcat(message, type);
    strcat(message, ",");
    strcat(message, ques);
    strcat(message, ",");
    strcat(message, ans);
    socketSend(TMclient, message, "check answer");

    // Receive QB response
    char response[BUFFERSIZE];
    int response_bytes = recv(TMclient, response, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.");
        exit(EXIT_FAILURE);
    }
    response[response_bytes] = '\0';
    printf("[+] QB response '%s' received successfully.\n", response);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");
    
    // If answer graded by QB is correct
    if (strcmp(response, "correct") == 0) {
        isCorrect = 1;
    } else if (strcmp(response, "wrong") == 0) {
        isCorrect = 0;
    }
    close(TMclient);
    return isCorrect;
}

// Create TM client to send request from QB to get answer
char* handleQBgetAns(char *type, char *ques) {
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    strcat(message, "get,");
    strcat(message, "answer,");
    strcat(message, type);
    strcat(message, ",");
    strcat(message, ques);
    socketSend(TMclient, message, "get answer");
    
    // Receive QB response
    char *correctAns = malloc(BUFFERSIZE);
    int response_bytes = recv(TMclient, correctAns, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.");
        exit(EXIT_FAILURE);
    }
    correctAns[response_bytes] = '\0';
    printf("[+] QB response '%s' received successfully.\n", correctAns);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");

    close(TMclient);
    return correctAns;
}

// Socket send message
void socketSend(int socket, char *message, char *type) {
    if (send(socket, message, strlen(message), 0) < 0) {
        printf("[-] Message '%s' failed to send.", type);
        exit(EXIT_FAILURE);
    }
    printf("[+] Message '%s' sent successfully.\n", type);
}