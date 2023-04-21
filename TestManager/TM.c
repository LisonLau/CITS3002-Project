// Server side C/C++ program to demonstrate Socket
// programming
// src: https://www.geeksforgeeks.org/socket-programming-cc/ 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT        8080
#define LINE_SIZE   50

char *username = "user1";
char *password = "pass1";

bool checkLoginDetails() {
    FILE *fin = fopen("students.csv", "r");
    if (fin == NULL) {
        printf("Cannot retrieve login details.");
        exit(EXIT_FAILURE);
    }
    char line[LINE_SIZE];
    int match = 0, authenticateUser = -1, authenticatePass = -50;
    bool authenticated = false;
    while (fgets(line, sizeof line, fin) != NULL && !authenticated) {
        char *token = strtok(line, ",");
        while (token != NULL && !authenticated) {
            // cross check the user name and password
            if (strcmp(username, token) == 0) {
                authenticateUser = match;
            }
            if (strcmp(password, token) == 0) {
                authenticatePass = match;
            }
            if (authenticatePass == authenticateUser) {
                authenticated = true;
                break;
            }
            token = strtok(NULL, "\n");
        }
        match++;
    }
    fclose(fin);
    if (authenticated) {
        return true;
    }
    return false;
}

int main(int argc, char const* argv[]) {   
    if (argc > 1) {
		printf("Usage: ./tm\n");
		exit(EXIT_FAILURE);
    }
    // bool ans = checkLoginDetails();
    // if (ans) {
    //     printf("Found user \n");
    // } else {
    //     printf("Didn't find user \n");
    // }
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    // Attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))< 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
  
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
} 