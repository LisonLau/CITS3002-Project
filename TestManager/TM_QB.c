#include "TM.h"

void handleQBgetFile(char *filename) {
    int clisockfd;
    struct sockaddr_in qb_addr;

    // Create a client socket
    if ((clisockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in creating TM client socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] TM client socket created to get file.\n");

    // Set server address and port
    qb_addr.sin_addr.s_addr = inet_addr(clientIpAddr);
    qb_addr.sin_family      = AF_INET;
    qb_addr.sin_port        = htons(CLIENT_PORT);

    // Connect to server
    if (connect(clisockfd, (struct sockaddr *)&qb_addr, sizeof(qb_addr)) < 0) {
        perror("[-] Error in connecting to QB.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection to QB successful.\n");

    sendQBgetFile(clisockfd, filename);

    close(clisockfd);
}

void sendQBgetFile(int socket, char *filename) {
    char message[BUFFERSIZE] = "";
    strcat(message, "get");
    strcat(message, ",");
    strcat(message, filename);

    // Send the request for file message
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("[-] Message 'get file' failed to send.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Message 'get file' sent successfully.\n");

    // Receive the file from QB
    char filelines[BUFFERSIZE];
    int n;
    FILE *fp = fopen(filename, "wb"); 
    while ((n = recv(socket, filelines, BUFFERSIZE, 0)) > 0) {
        fwrite(filelines, sizeof(char), n, fp); 
    }
    fclose(fp);
    printf("[+] Question file '%s' received successfully.\n", filename);

    // Send acknowledgement for received data
    // char ack[BUFFERSIZE] = "ACK";
    // if (send(socket, ack, strlen(ack), 0) < 0) {
    //     perror("[-] Acknowledgement failed to send.");
    //     exit(EXIT_FAILURE);
    // }
    // printf("[+] Acknowledgement sent successfully.\n");
}

int handleQBcheck(char *type, char *ques, char *ans) {
    int isCorrect = 0;
    int clisockfd;
    struct sockaddr_in qb_addr;

    // Create a client socket
    if ((clisockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in creating TM client socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] TM client socket created to check answer.\n");

    // Set server address and port
    qb_addr.sin_addr.s_addr = INADDR_ANY;
    qb_addr.sin_family      = AF_INET;
    qb_addr.sin_port        = htons(CLIENT_PORT);

    // Connect to server
    if (connect(clisockfd, (struct sockaddr *)&qb_addr, sizeof(qb_addr)) < 0) {
        perror("[-] Error in connecting to QB.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection to QB successful.\n");
    
    isCorrect = sendQBCheck(clisockfd, type, ques, ans);

    close(clisockfd);
    return isCorrect;
}

int sendQBCheck(int socket, char *type, char *question, char *answer) {
    char message[BUFFERSIZE] = "";
    strcat(message, type);
    strcat(message, ",");
    strcat(message, question);
    strcat(message, ",");
    strcat(message, answer);

    // Send the message
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("[-] Message 'check answer' failed to send.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Message 'check answer' sent successfully.\n");

    // Receive QB response
    char response[BUFFERSIZE];
    int response_bytes = recv(socket, response, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.");
        exit(EXIT_FAILURE);
    }
    response[response_bytes] = '\0';
    printf("[+] QB response received successfully.\n");

    // Send acknowledgement for received data
    // char ack[BUFFERSIZE] = "ACK";
    // if (send(socket, ack, strlen(ack), 0) < 0) {
    //     perror("[-] Acknowledgement failed to send.");
    //     exit(EXIT_FAILURE);
    // }
    // printf("[+] Acknowledgement sent successfully.\n");
    
    // If answer graded by QB is correct
    if (strcmp(response, "correct") == 0) {
        return 1;
    } else if (strcmp(response, "wrong") == 0) {
        return 0;
    }
    return 0;
}

char* handleQBgetAns(char *type, char *ques) {
    char *correctAns;
    int clisockfd;
    struct sockaddr_in qb_addr;

    // Create a client socket
    if ((clisockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in creating TM client socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] TM client socket created to check answer.\n");

    // Set server address and port
    qb_addr.sin_addr.s_addr = INADDR_ANY;
    qb_addr.sin_family      = AF_INET;
    qb_addr.sin_port        = htons(CLIENT_PORT);

    // Connect to server
    if (connect(clisockfd, (struct sockaddr *)&qb_addr, sizeof(qb_addr)) < 0) {
        perror("[-] Error in connecting to QB.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection to QB successful.\n");
    
    correctAns = sendQBgetAns(clisockfd, type, ques);

    close(clisockfd);
    return correctAns;
}

char* sendQBgetAns(int socket, char *type, char *question) {
    char message[BUFFERSIZE] = "";
    strcat(message, "get");
    strcat(message, ",");
    strcat(message, "answer");
    strcat(message, ",");
    strcat(message, type);
    strcat(message, ",");
    strcat(message, question);

    // Send the message
    if (send(socket, message, strlen(message), 0) < 0) {
        perror("[-] Message 'get answer' failed to send.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Message 'get answer' sent successfully.\n");

    // Receive QB response
    char *response = malloc(BUFFERSIZE);
    int response_bytes = recv(socket, response, BUFFERSIZE, 0);
    if (response_bytes < 0) {
        perror("[-] Failed to receive QB response.");
        exit(EXIT_FAILURE);
    }
    response[response_bytes] = '\0';
    printf("[+] QB response received successfully.\n");

    return response;
}