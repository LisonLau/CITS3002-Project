#include "TM.h"

int createTMclient() {
    int TMclient;
    struct sockaddr_in QBaddress;

    // Create a client socket
    if ((TMclient = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in creating TM client socket.\n");
        exit(EXIT_FAILURE);
    }
    printf("[+] TM client socket created to send request.\n");

    // Set server address and port
    QBaddress.sin_addr.s_addr = inet_addr(CLIENT_HOST);
    QBaddress.sin_family      = AF_INET;
    QBaddress.sin_port        = htons(CLIENT_PORT);

    // Connect to server
    if (connect(TMclient, (struct sockaddr *)&QBaddress, sizeof(QBaddress)) < 0) {
        perror("[-] Error in connecting to QB.\n");
        exit(EXIT_FAILURE);
    }
    printf("------- Connection to QB successful -------\n");
    return TMclient;
}

// Create TM client to send request from QB for question file
void handleQBgetFile(char *filename) {
    int TMclient = createTMclient();

    // Send the request for file message
    char message[BUFFERSIZE] = "";
    strcat(message, "get@");
    strcat(message, filename);
    socketSend(TMclient, message, "get file");

    // Receive QB acknowledgement for sent request
    receiveACK(TMclient, message, "get file");

    // Receive the file from QB
    char filelines[BUFSIZ];
    FILE *fp = fopen(filename, "wb"); 
    if (fp == NULL) {
        fprintf(stderr, "[-] Error: Failed to open file '%s' for writing.\n", filename);
        exit(EXIT_FAILURE);
    }
    int bytes = recv(TMclient, filelines, BUFSIZ, 0);
    fwrite(filelines, sizeof(char), bytes, fp); 
    fclose(fp);
    printf("[+] Question file '%s' received successfully.\n", filename);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");

    close(TMclient);
    printf("--------- Connection to QB closed ---------\n");
}

// Create TM client to send request from QB to check answer
int handleQBcheck(char *type, char *ques, char *ans) {
    int isCorrect = -1;
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    strcat(message, type);
    strcat(message, "@");
    strcat(message, ques);
    strcat(message, "@");
    strcat(message, ans);
    socketSend(TMclient, message, "check answer");

    // Receive QB acknowledgement for sent request
    receiveACK(TMclient, message, "check answer");

    // Receive QB response
    char response[BUFFERSIZE];
    int response_bytes = recv(TMclient, response, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.\n");
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
    printf("--------- Connection to QB closed ---------\n");
    return isCorrect;
}

// Create TM client to send request from QB to get answer
char* handleQBgetAns(char *type, char *ques) {
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    strcat(message, "get@");
    strcat(message, "answer@");
    strcat(message, type);
    strcat(message, "@");
    strcat(message, ques);
    socketSend(TMclient, message, "get answer");

    // Receive QB acknowledgement for sent request
    receiveACK(TMclient, message, "get answer");
    
    // Receive QB response
    char *correctAns = malloc(BUFFERSIZE);
    if (correctAns == NULL) {
        perror("[!] Error: Failed to allocate memory for response.\n");
        exit(EXIT_FAILURE);
    }
    int response_bytes = recv(TMclient, correctAns, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.\n");
        exit(EXIT_FAILURE);
    }
    correctAns[response_bytes] = '\0';
    printf("[+] QB response '%s' received successfully.\n", correctAns);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");

    close(TMclient);
    printf("--------- Connection to QB closed ---------\n");
    return correctAns;
}

// Socket send message
void socketSend(int socket, char *message, char *type) {
    if (send(socket, message, strlen(message), 0) < 0) {
        fprintf(stderr, "[-] Message '%s' failed to send.", type);
        exit(EXIT_FAILURE);
    }
    printf("[+] Message '%s' sent successfully.\n", type);
}

// Receive QB acknowledgement for sent request
void receiveACK(int TMclient, char *message, char *type) {
    char ack[BUFFERSIZE] = "";
    while (strcmp(ack, "ACK") != 0) {
        int ack_bytes = recv(TMclient, ack, BUFFERSIZE, 0);
        if (ack_bytes < 0) {
            perror("[-] Acknowledgement from QB not received.\n");
            exit(EXIT_FAILURE);
        }
        ack[ack_bytes] = '\0';
        if (strcmp(ack, "ACK") == 0) {
            printf("[+] Acknowledgement from QB received.\n");
            break;
        } else {
            printf("[-] Acknowledgement from QB not received.\n");
            printf("[.] Retrying in 5 seconds...\n");
            sleep(5);
            socketSend(TMclient, message, type);
        }
    }
}