#include "TM.h"

void runTMforWeb() {
    int                 opt = 1;
    int                 sersockfd, newsockfd, valread;
    struct sockaddr_in  ser_addr;
    socklen_t           ser_addrsize;
    char                buffer[BUFFERSIZE];

    // Create socket file descriptor
    if ((sersockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket created.\n");

    // Set socket options
    if (setsockopt(sersockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    ser_addr.sin_family      = AF_INET;
    ser_addr.sin_port        = htons(PORT);
    ser_addr.sin_addr.s_addr = INADDR_ANY; //inet_addr("192.168.1.123");

    // Bind socket to port
    ser_addrsize = sizeof(ser_addr);
    if (bind(sersockfd, (struct sockaddr*)&ser_addr, ser_addrsize) < 0) {
        perror("[-] Error in binding.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");

    // Listen for incoming connections
    if (listen(sersockfd, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening...\n");

    // Accept incoming connections
    while (1) {
        if ((newsockfd = accept(sersockfd, (struct sockaddr*)&ser_addr, (socklen_t*)&ser_addrsize)) < 0) {
            perror("[-] Error in accepting.");
            exit(EXIT_FAILURE);
        }

        // Read HTTP request
        memset(buffer, 0, BUFFERSIZE);
        if ((valread = read(newsockfd, buffer, BUFFERSIZE)) < 0) {
            perror("[-] Error in reading HTTP request.");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buffer);
        handleUserLogin(newsockfd, buffer);
        close(newsockfd);
    }
}

void handleUserLogin(int socket, char *buffer) {
    char *form = strstr(buffer, "uname=");
    // Display login page
    if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
        char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\">\
                           <label for=\"uname\">Username : </label>\
                           <input type=\"text\" name=\"uname\" value=\"\" required><br><br>\
                           <label for=\"pword\">Password : </label>\
                           <input type=\"text\" name=\"pword\" value=\"\" required><br><br>\
                           <button type=\"submit\">Login</button></form></body></html>";
        sendResponse(socket, loginHTML);
    } 
    // Extract the username and password from the form data
    else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        char username[MAX_USERNAME_LENGTH] = {0};
        char password[MAX_PASSWORD_LENGTH] = {0};
        sscanf(form, "uname=%[^&]&pword=%s", username, password);
        // User successfully logged in, display question page
        if (authenticateUsers(username, password)) {
            handleDisplayQuestion(socket, username, password);
        } 
        // User failed to logged in, ask for login attempt
        else {
            char *loginFAILED  = "<html><body><h1>Login</h1><form method=\"post\">\
                                  <label for=\"uname\">Username : </label>\
                                  <input type=\"text\" name=\"uname\" required><br><br>\
                                  <label for=\"pword\">Password : </label>\
                                  <input type=\"text\" name=\"pword\" required><br>\
                                  <p>Login failed. Try again.</p><br>\
                                  <button type=\"submit\">Login</button></form></body></html>";
            sendResponse(socket, loginFAILED);
        }
    }
    // Display 404 error page
    else {
        char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
        sendResponse(socket, errorHTML);
    }
}

void sendResponse(int socket, char *message) {
    char response[BUFFERSIZE] = {0};
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(message), message);
    send(socket, response, strlen(response), 0);
}
