// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

// Transfer files and send messages using TCP socket in C
// Reference - https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

#include "TM.h"

/**
 * @brief create the TM client socket to connect to QB server
 * @return int the TM client socket file descriptor
 */
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

/**
 * @brief create TM client to send request from QB for question file
 * @param filename student's custom filename to store the questions
 */
void handleQBgetFile(char *filename) {
    int TMclient = createTMclient();

    // Send the request for file message
    char message[BUFFERSIZE] = "";
    snprintf(message, sizeof(message), "get_file@%s", filename);
    socketSend(TMclient, message, "get file");

    // Receive QB acknowledgement for sent request
    receiveACK(TMclient, message, "get file");

    // Receive the file from QB
    char filelines[FILESIZE];
    FILE *fp = fopen(filename, "wb"); 
    if (fp == NULL) {
        fprintf(stderr, "[-] Error: Failed to open file '%s' for writing.\n", filename);
        exit(EXIT_FAILURE);
    }
    int bytes = recv(TMclient, filelines, FILESIZE, 0);
    fwrite(filelines, sizeof(char), bytes, fp); 
    fclose(fp);
    printf("[+] Question file '%s' received successfully.\n", filename);

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");

    close(TMclient);
    printf("--------- Connection to QB closed ---------\n");
}

/**
 * @brief create TM client to send request from QB to check answer
 * @param type type of question
 * @param question the question 
 * @param anwer the student answer
 * @return int 1 if student answer is correct, 0 otherwise
 */
int handleQBcheck(char *type, char *question, char *answer) {
    int isCorrect = -1;
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    snprintf(message, sizeof(message), "check_answer@%s@%s@%s", type, question, answer);
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
    char *mark = strtok(response, "@");
    char *output = strtok(NULL, "@");

    // Send acknowledgement for received data
    char ack[BUFFERSIZE] = "ACK";
    socketSend(TMclient, ack, "ACKNOWLEDGEMENT");
    
    // MCQ - Check if answer is correct or wrong
    if (strcmp(mark, "correct") == 0) {
        isCorrect = 1;
    } else if (strcmp(mark, "wrong") == 0) {
        isCorrect = 0;
    }

    // PCQ - Store answer
    if (strcmp(type, "pcqc") == 0 || strcmp(type, "pcqpy") == 0 ) {
        if (output != NULL){
            strcpy(answer, output);
        }
    }

    close(TMclient);
    printf("--------- Connection to QB closed ---------\n");
    return isCorrect;
}

/**
 * @brief create TM client to request QB to get answer
 * @param type type of question
 * @param question the question 
 * @return char* the correct answer to the given question
 */
char* handleQBgetAns(char *type, char *question) {
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    snprintf(message, sizeof(message), "get_answer@%s@%s", type, question);
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

/**
 * @brief create TM client to request QB to get image
 * @param type type of question
 * @param question the question
 * @param imageName name of image file
 */
void handleQBgetImg(char *type, char *question, char *imageName) {
    int TMclient = createTMclient();

    // Send the message
    char message[BUFFERSIZE] = "";
    snprintf(message, sizeof(message), "get_image@%s@%s", type, question);
    socketSend(TMclient, message, "get image");
    
    // Receive image from QB
    char imageData[HTMLSIZE];
    int  imageBytes;
    FILE *imageFile = fopen(imageName, "wb"); 
    if (imageFile == NULL) {
        fprintf(stderr, "[-] Error: Failed to open image for writing.\n");
        exit(EXIT_FAILURE);
    }

    while ((imageBytes = recv(TMclient, imageData, HTMLSIZE, 0)) > 0) {
        fwrite(imageData, sizeof(char), imageBytes, imageFile);
    }
    fclose(imageFile);
    printf("[+] Image received successfully.\n");

    close(TMclient);
    printf("--------- Connection to QB closed ---------\n");
}

/**
 * @brief handles sending socket message
 * @param TMclient TM client socket file descriptor
 * @param message the string message to be sent
 * @param type the type of request
 */
void socketSend(int TMclient, char *message, char *type) {
    if (send(TMclient, message, strlen(message), 0) < 0) {
        fprintf(stderr, "[-] Message '%s' failed to send.", type);
        exit(EXIT_FAILURE);
    }
    printf("[+] Message '%s' sent successfully.\n", type);
}

/**
 * @brief receive QB acknowledgement for sent request
 * @param TMclient TM client socket file descriptor
 * @param message the string message to be sent
 * @param type the type of request
 */
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
