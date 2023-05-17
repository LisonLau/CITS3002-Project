// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "TM.h"

/**
 * @brief create and run TM server socket for web browser clients
 */
void runTMforWeb() {
    int                 opt = 1;
    int                 max_sd, activity, newsockfd, sockfd, valread;
    int                 client_socket[MAX_CLIENTS] = {0};
    struct sockaddr_in  addr;
    socklen_t           addrsize;
    fd_set              readset;
    char                HTTPrequest[BUFFERSIZE];
    int                 isLoggedIn = 0;
    struct timeval      timeout;
    timeout.tv_sec = 10; // wait up to 10 seconds 
    timeout.tv_usec = 0;
  
    // Create socket file descriptor
    if ((TMserver = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    } 
    printf("[+] Server socket created.\n");

    // Set socket options
    if (setsockopt(TMserver, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(SERVER_PORT);

    // Bind socket to port
    addrsize = sizeof(addr);
    if (bind(TMserver, (struct sockaddr*)&addr, addrsize) < 0) {
        perror("[-] Error in binding");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");
    printf("[+] Server socket, fd: %d\n", TMserver);

    // Listen for incoming connections
    if (listen(TMserver, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening as %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

    // Accept incoming connections
    while (1) {
        // Clear the socket set
        FD_ZERO(&readset);

        // Add master socket to set
        FD_SET(TMserver, &readset);
        max_sd = TMserver;

        // Adding child sockets to the sets
        for (int i = 0; i < MAX_CLIENTS; i++){
            sockfd = client_socket[i];
            if (sockfd > 0) FD_SET(sockfd, &readset);
            if (sockfd > max_sd)    max_sd = sockfd;
        }

        // Waiting for one of the sockets to do something, waits indefinitely
        printf("[+] Waiting...\n");
        activity = select(max_sd + 1, &readset, NULL, NULL, &timeout);
        if (activity < 0 && errno != EINTR){
            perror("[-] Socket connection error.\n");
            exit(EXIT_FAILURE);
        }

        // When something a client request comes into the server
        if (FD_ISSET(TMserver, &readset)) {
            if ((newsockfd = accept(TMserver, (struct sockaddr*)&addr, &addrsize)) < 0) {
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
                memset(HTTPrequest, 0, BUFFERSIZE);
                getpeername(sockfd, (struct sockaddr*)&addr, &addrsize);
                if ((valread = read(sockfd, HTTPrequest, BUFFERSIZE))  == 0) {
                    // This client has disconnected
                    printf("[.] A client disconnected, ip: %s, port: %d\n",
                            inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
                    client_socket[i] = 0;
                    shutdown(sockfd, SHUT_RDWR);
                    close(sockfd);
                } 
                else {
                    printf("%s\n\n", HTTPrequest);

                    // Handle user login if user has not logged in
                    isLoggedIn = checkLoggedIn(inet_ntoa(addr.sin_addr), 0);
                    printf("[!] %s isLoggedIn: %d\n",inet_ntoa(addr.sin_addr), isLoggedIn);

                    // If student is not logged in
                    if (isLoggedIn == -1) {
                        isLoggedIn = handleUserLogin(sockfd, inet_ntoa(addr.sin_addr), HTTPrequest);
                    }

                    // If a student logs out
                    if (strstr(HTTPrequest, "POST / HTTP/1.1") != NULL && strstr(HTTPrequest, "logout=Logout") != NULL) {
                        char *loginHTML = {0};
                        loginHTML = getLoginHTML(loginHTML, 0);
                        sendHTMLpage(sockfd, loginHTML);
                        if (loginHTML != NULL) {
                            free(loginHTML);
                            loginHTML = NULL;
                        }
                        isLoggedIn = -1;
                        int index = checkLoggedIn(inet_ntoa(addr.sin_addr), 1);
                        students[index].loggedIn = 0;
                        strcpy(students[index].ipAddress, "");
                        continue;
                    }

                    // If student is logged in
                    if (isLoggedIn) {
                        // Find that student
                        int index = checkLoggedIn(inet_ntoa(addr.sin_addr), 1);
                        Students currStudent = students[index];
                        int quesIdx = currQuestion[index];
                        printf("[.] Student is: %s, index:%i quesIdx:%i\n", currStudent.username, index, quesIdx);
            
                        // Handle display finish page after test is done
                        if (currStudent.allocated[quesIdx].isDone == 1 && quesIdx >= MAX_QUESTIONS-1) {
                            char *finishHTML = {0};
                            finishHTML = getFinishHTML(finishHTML, currStudent.grade);
                            sendHTMLpage(sockfd, finishHTML);
                            if (finishHTML != NULL) {
                                free(finishHTML);
                                finishHTML = NULL;
                            }
                        }
                        // Handle display question page of current question
                        else {
                            // Increment quesIdx on NEXT button press
                            if (strstr(HTTPrequest, "next=Next") != NULL)   currQuestion[index]++;
                            // Decrement quesIdx on BACK button press
                            if (strstr(HTTPrequest, "back=Back") != NULL)   currQuestion[index]--;
                            
                            handleDisplayTest(sockfd, HTTPrequest, &students[index], index);
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief handles displaying login page to web browser
 * @param socket the socket file descriptor
 * @param ip the IP address
 * @param HTTPrequest the HTTP request received from client web browser
 * @return int 1 if student successfully login, 0 if failed
 */
int handleUserLogin(int socket, char *ip, char *HTTPrequest) {
    char *form = strstr(HTTPrequest, "uname=");
    // Display login page
    if (strstr(HTTPrequest, "GET / HTTP/1.1") != NULL) {
        char *loginHTML = {0};
        loginHTML = getLoginHTML(loginHTML, 0);
        sendHTMLpage(socket, loginHTML);
        if (loginHTML != NULL) {
            free(loginHTML);
            loginHTML = NULL;
        }
    } 
    // Extract the username and password from the form data
    else if (strstr(HTTPrequest, "POST / HTTP/1.1") != NULL) {
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
            sendHTMLpage(socket, loginHTML);
            if (loginHTML != NULL) {
                free(loginHTML);
                loginHTML = NULL;
            }
        }
    }
    // Display 404 error page
    else {
        char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
        if (send(socket, errorHTML, strlen(errorHTML), 0) < 0) {
            perror("[-] Error sending HTML page.");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

/**
 * @brief checked whether the student is logged in
 * @param var the TODO LIANNE
 * @param getIndex 1 if function is getting the student index, 0 otherwise
 * @return int 
 */
int checkLoggedIn(char *var, int getIndex) {
    for (int i = 0; i < MAX_STUDENTS; i++) {
        // Checks if a student is associated with this IP and is logged in
        if (getIndex == 1 && (strcmp(students[i].username, var) == 0 || strcmp(students[i].ipAddress, var) == 0)) {
            return i; // returns index of the student
        }
        else if (getIndex == 0 && strcmp(students[i].ipAddress, var) == 0 && students[i].loggedIn == 1) {
            printf("ip: %s, loggedIn: %d", students[i].ipAddress, students[i].loggedIn);
            return 1; // returns 1 for true user is logged in
        }
    }
    return -1; // not 0 because i need to differentiate between index 0 and false
}

/**
 * @brief handles TM socket sending the HTML page to the web browser
 * @param TMsocket TM socket file descriptor
 * @param message the HTML message to be send
 * @param sendImage 1 if socket is sending an image, 0 otherwise
 */
void sendHTMLpage(int TMsocket, char *message) {
    char response[HTMLSIZE] = {0};
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(message), message);
    if (send(TMsocket, response, strlen(response), 0) < 0) {
        fprintf(stderr, "[-] Error sending HTML page.");
        exit(EXIT_FAILURE);
    }
}

void sendImagePage(int TMsocket) {
    // Read the image file
    FILE* imageFile = fopen("tempImg.png", "rb");
    if (imageFile == NULL) {
        fprintf(stderr, "[-] Error opening image file.");
        exit(EXIT_FAILURE);
    }

    // Get size of image file
    fseek(imageFile, 0, SEEK_END);
    long imageSize = ftell(imageFile);
    fseek(imageFile, 0, SEEK_SET);

    printf("%ld\n", imageSize);

    // Allocate memory to store the image
    char* imageData  = (char*)malloc(imageSize);
    if (imageData  == NULL) {
        fprintf(stderr, "[-] Error allocating memory for image buffer.");
        exit(EXIT_FAILURE);
    }

    // Read image data into the buffer
    if (fread(imageData, 1, imageSize, imageFile) != imageSize) {
        fprintf(stderr, "[-] Error reading image file.");
        exit(EXIT_FAILURE);
    }
    fclose(imageFile);

    // Send HTTP response headers
    char responseHeaders[BUFFERSIZE];
    snprintf(responseHeaders, sizeof(responseHeaders),\
    "HTTP/1.1 200 OK\nContent-Type: image/png\nContent-Length: %ld\n\n", imageSize);
    if (send(TMsocket, responseHeaders, strlen(responseHeaders), 0) < 0) {
        fprintf(stderr, "[!] Failed to send HTML response headers.");
        exit(EXIT_FAILURE);
    }

    // Send the image
    if (send(TMsocket, imageData, imageSize, 0) < 0) {
        fprintf(stderr, "[!] Failed to send image data.");
        exit(EXIT_FAILURE);
    }

    // Free the allocated memory
    free(imageData);
}
