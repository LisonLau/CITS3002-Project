#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFFERSIZE   1024
#define USERNAME    "user1"
#define PASSWORD    "pass1"

int main() {
    char *host  = "127.0.0.1";  // host
    int port    = 8080;         // port
    int opt = 1;
    int n;
    char buffer[BUFFERSIZE];
    int sockfd, newsockfd;
    socklen_t addr_size;
    struct sockaddr_in ser_addr, cli_addr;

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket created.\n");

    // Clear the serv_addr struct
    memset((char *) &ser_addr, 0, sizeof(ser_addr));

    // Attach socket to port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in attaching socket to port.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Attaching socket to port successful.\n");

    ser_addr.sin_family      = AF_INET;
    ser_addr.sin_port        = htons(port);
    ser_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0) {
        perror("[-] Error in binding.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");

    // Listen for incoming connections
    if (listen(sockfd, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening...\n");

    // Accept incoming connections
    addr_size = sizeof(cli_addr);
    if ((newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &addr_size)) < 0) {
        perror("[-] Error in accepting.");
        exit(EXIT_FAILURE);
    }

    // Create custom filename
    char filename[100] = "";
    strcat(filename, USERNAME);
    strcat(filename, PASSWORD);
    strcat(filename, ".csv");

    // Receive file contents and store into file
    FILE *fp = fopen(filename, "wb");
    while ((n = recv(newsockfd, buffer, BUFFERSIZE, 0)) > 0) {
        fwrite(buffer, sizeof(char), n, fp);
    }
    printf("[+] File received successfully.\n");
    fclose(fp);     // Close the file

    return EXIT_SUCCESS;
}