#include "TM.h"

void runTMforWeb() {
    int                 opt = 1;
    int                 max_sd, activity, sersockfd, newsockfd, sockfd, valread;
    int                 client_socket[MAX_CLIENTS] = {0};
    // struct hostent      *hostInfo;
    // struct in_addr      **addr_list;
    struct sockaddr_in  addr;
    socklen_t           addrsize;
    fd_set              readset;
    char                buffer[BUFFERSIZE];
    // char                hostname[255];
    int                 isLoggedIn = 0;

    // Retrieving HOST IP address
    // gethostname(hostname, 255);
    // hostInfo = gethostbyname(hostname);
    // addr_list = (struct in_addr **)hostInfo->h_addr_list;
    // HOST = inet_ntoa(*addr_list[0]);
    // if (strcmp(HOST, "127.0.0.1") == 0) {
    //     HOST = inet_ntoa(*addr_list[1]);
    // }

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

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    addrsize = sizeof(addr);
    if (bind(sersockfd, (struct sockaddr*)&addr, addrsize) < 0) {
        perror("[-] Error in binding.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");
    printf("[+] Server socket, fd: %d\n", sersockfd);

    // Listen for incoming connections
    if (listen(sersockfd, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening as %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    // Accept incoming connections
    while (1) {
        // Clear the socket set
        FD_ZERO(&readset);

        // Add master socket to set
        FD_SET(sersockfd, &readset);
        max_sd = sersockfd;

        // Adding child sockets to the sets
        for (int i = 0; i < MAX_CLIENTS; i++){
            sockfd = client_socket[i];
            if (sockfd > 0) {FD_SET(sockfd, &readset);}
            if (sockfd > max_sd) {max_sd = sockfd;}
        }

        // Waiting for one of the sockets to do something, waits indefinitely
        printf("[+] Waiting...\n");
        activity = select(max_sd + 1, &readset, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR){
            perror("[-] Socket connection error.\n");
            exit(EXIT_FAILURE);
        }

        // When something a client request comes into the server
        if (FD_ISSET(sersockfd, &readset)) {
            if ((newsockfd = accept(sersockfd, (struct sockaddr*)&addr, &addrsize)) < 0) {
                perror("[-] Error in accepting.");
                exit(EXIT_FAILURE);
            }
            printf("[+] New connection, socket fd: %d, ip: %s, port: %d\n",
                    newsockfd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

            // Adds the new socket to array of sockets 
            for (int i = 0; i < MAX_STUDENTS; i++) {  
                // If position is empty 
                if (client_socket[i] == 0) {  
                    client_socket[i] = newsockfd;  
                    printf("[+] Adding to list of sockets as %d\n" , i); 
                    break;  
                }
            }
        }

        // Going through the list of connected clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sockfd = client_socket[i];
            if (FD_ISSET(sockfd, &readset)) {

                // Read HTTP request
                memset(buffer, 0, BUFFERSIZE);
                getpeername(sockfd, (struct sockaddr*)&addr, &addrsize);
                if ((valread = read(sockfd, buffer, BUFFERSIZE))  == 0) {
                    // This client has disconnected
                    printf("[-] A client disconnected, ip: %s, port: %d\n",
                            inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                    client_socket[i] = 0;
                    shutdown(sockfd, SHUT_RDWR);
                    close(sockfd);
                } 
                else {
                    printf("%s\n", buffer);

                    // Handle user login if user has not logged in
                    isLoggedIn = checkLoggedIn(inet_ntoa(addr.sin_addr), 0);
                    printf("[!] %s isLoggedIn: %d\n",inet_ntoa(addr.sin_addr), isLoggedIn);

                    // If student is not logged in
                    if (isLoggedIn == -1) {
                        isLoggedIn = handleUserLogin(sockfd, inet_ntoa(addr.sin_addr), buffer);
                    }

                    // If a student logs out
                    if (strstr(buffer, "POST / HTTP/1.1") != NULL && strstr(buffer, "logout=Logout") != NULL) {
                        char *loginHTML = {0};
                        loginHTML = getLoginHTML(loginHTML, 0);
                        sendResponse(sockfd, loginHTML);
                        free(loginHTML);
                        isLoggedIn = -1;
                        int index = checkLoggedIn(inet_ntoa(addr.sin_addr), 1);
                        students[index].loggedIn = 0;
                        strcpy(students[index].ipAddress, "");
                        printf("students[index].ipAddress %s\n", students[index].ipAddress);
                        printf("index %d\n", index);
                        printf("students[index].loggedIn %d\n", students[index].loggedIn);
                    }

                    // If student is logged in
                    if (isLoggedIn) {
                        // Find that student
                        int index = checkLoggedIn(inet_ntoa(addr.sin_addr), 1);
                        Students currStudent = students[index];
                        int quesIdx = currStudent.quesIdx;
                        printf("[.] Student is: %s, index:%i quesIdx:%i\n", currStudent.username, index, quesIdx);
            
                        // Handle display finish page after test is done
                        if (currStudent.allocated[quesIdx].isDone == 1 && quesIdx >= MAX_QUESTIONS-1) {
                            char *finishHTML = {0};
                            finishHTML = getFinishHTML(sockfd, buffer, currStudent.grade, finishHTML);
                            sendResponse(sockfd, finishHTML);
                            free(finishHTML);
                        }
                        // Handle display question page of current question
                        else {
                            handleDisplayTest(sockfd, buffer, &students[index]);
                        }
                    }
                }
            }
        }
    }
}

int handleUserLogin(int socket, char *ip, char *buffer) {
    char *form = strstr(buffer, "uname=");
    // Display login page
    if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
        char *loginHTML = {0};
        loginHTML = getLoginHTML(loginHTML, 0);
        sendResponse(socket, loginHTML);
        free(loginHTML);
    } 
    // Extract the username and password from the form data
    else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
        char uname[MAX_USERNAME_LENGTH] = {0};
        char pword[MAX_PASSWORD_LENGTH] = {0};
        sscanf(form, "uname=%[^&]&pword=%s", uname, pword);
        // User successfully logged in, display question page
        if (authenticateUsers(uname, pword)) {
            printf("[+] User has successfully logged in.\n");
            int match = checkLoggedIn(uname, 1);
            strcpy(students[match].ipAddress, ip);
            students[match].loggedIn = 1;
            return 1;
        } 
        // User failed to logged in, ask for login attempt
        else {
            char *loginHTML = {0};
            loginHTML = getLoginHTML(loginHTML, 1);
            sendResponse(socket, loginHTML);
            free(loginHTML);
        }
    }
    // Display 404 error page
    else {
        char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
        send(socket, errorHTML, strlen(errorHTML), 0);
    }
    return 0;
}

int checkLoggedIn(char *var, int getIndex) {
    for (int i = 0; i < MAX_STUDENTS; i++) {
        // Checks if a student is associated with this IP and is logged in
        if (getIndex && (strcmp(students[i].username, var) == 0 || strcmp(students[i].ipAddress, var) == 0)) {
            return i; // returns index of the student
        }
        else if (!getIndex && strcmp(students[i].ipAddress, var) == 0 && students[i].loggedIn == 1) {
            return 1; // returns 1 for true user is logged in
        }
    }
    return -1; // not 0 because i need to differentiate between index 0 and false
}

void sendResponse(int socket, char *message) {
    char response[HTMLSIZE] = {0};
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(message), message);
    send(socket, response, strlen(response), 0);
}
