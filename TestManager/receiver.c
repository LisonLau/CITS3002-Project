#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h>

#define BUFFERSIZE   1024
#define USERNAME    "user1"
#define PASSWORD    "pass1"

int getQuestionFile() {
    // char *host  = "127.0.0.1";  // host
    int port    = 8080;         // port
    int opt     = 1;
    int max_clients = 30;
    int n, sd, max_sd, activity, value;
    char buffer[BUFFERSIZE];
    int master_sockfd, newsockfd, client_socket[30];
    socklen_t addr_size;
    fd_set readfds;
    struct sockaddr_in ser_addr, cli_addr;

    // Initialise all clien_socket[] to 0, so we don't check
    for (int i = 0; i < max_clients; i++){
        client_socket[i] = 0;
    }

    // Create socket file descriptor
    if ((master_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket created.\n");

    // Clear the serv_addr struct
    memset((char *) &ser_addr, 0, sizeof(ser_addr));

    // Set socket options
    if (setsockopt(master_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    ser_addr.sin_family      = AF_INET;
    ser_addr.sin_port        = htons(port);
    ser_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(master_sockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0) {
        perror("[-] Error in binding.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");

    // Listen for incoming connections, maximum 10 pending connections
    if (listen(master_sockfd, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening...\n");

    // Accept incoming connections
    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add master socket to set
        FD_SET(master_sockfd, &readfds);
        max_sd = master_sockfd;

        // Adding child sockets to the sets
        for (int i = 0; i < max_clients; i++){
            sd = client_socket [i];
            if (sd > 0){
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd){
                max_sd = sd;
            }
        }
        // Waiting for one of the sockets to do something, waits indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR){
            printf("[-] Socket connection error.");
        }

        // When something happens on the master socket = incoming connection
        if (FD_ISSET(master_sockfd, &readfds)){
            addr_size = sizeof(cli_addr);
            if ((newsockfd = accept(master_sockfd, (struct sockaddr*)&cli_addr, &addr_size)) < 0) {
                perror("[-] Error in accepting.");
                exit(EXIT_FAILURE);
            }

            printf("[+] New connection, socket fd: %d, ip: %s, port: %d\n",
                    newsockfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            // Adds the new socket to array of sockets 
            for (int i = 0; i < max_clients; i++)  
            {  
                // If position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = newsockfd;  
                    printf("[+] Adding to list of sockets as %d\n" , i);   
                    break;  
                }  
            }  
        }
        
        for (int i = 0; i < max_clients; i++){
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                if ((value = read(sd, buffer, BUFFERSIZE)) == 0){
                    // One of the client's disconnected
                    getpeername(sd, (struct sockaddr*)&cli_addr, &addr_size);
                    printf("[-] Client disconnected, ip: %s, port: %d\n",
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                }
                // Incoming message
                // TODO: 
                else {
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
                    fclose(fp);      // Close the file
                }
            }
        }
    }
}

int main() {
    getQuestionFile();
    return EXIT_SUCCESS;
}