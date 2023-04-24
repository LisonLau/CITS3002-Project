#include "TM.h"

void getUserLogin() {
    // char *host  = "127.0.0.1";  // host
    int port    = 8080;         // port
    int opt     = 1;
    int max_clients = 30;
    int n, sd, max_sd, activity, valread;
    int ser_sockfd, newsockfd, client_socket[30], client_verified[30];
    struct sockaddr_in ser_addr, cli_addr;
    socklen_t addrsize;
    fd_set readfds;
    char buffer[BUFFERSIZE] = {0};

    // Initialise all clien_socket[] to 0, so we don't check
    for (int i = 0; i < max_clients; i++){
        client_socket[i] = 0;
        client_verified[i] = -1;
    }
    // Create socket file descriptor
    if ((ser_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket created.\n");

    // Clear the serv_addr struct
    memset((char *) &ser_addr, 0, sizeof(ser_addr));

    // Set socket options
    if (setsockopt(ser_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    ser_addr.sin_family      = AF_INET;
    ser_addr.sin_port        = htons(port);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket to port
    if (bind(ser_sockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0) {
        perror("[-] Error in binding.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Binding successful.\n");

    // Listen for incoming connections
    if (listen(ser_sockfd, 10) < 0) {
        perror("[-] Error in listening.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening...\n");

    // Accept incoming connections
    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add master socket to set
        FD_SET(ser_sockfd, &readfds);
        max_sd = ser_sockfd;

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
        printf("[+] Waiting...\n");
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR){
            printf("[-] Socket connection error.\n");
        }

        // When something happens on the master socket = incoming connection
        if (FD_ISSET(ser_sockfd, &readfds)){
            addrsize = sizeof(cli_addr);
            if ((newsockfd = accept(ser_sockfd, (struct sockaddr*)&cli_addr, &addrsize)) < 0) {
                perror("[-] Error in accepting connection: ");
                exit(EXIT_FAILURE);
            }
            printf("[+] New connection, socket fd: %d, ip: %s, port: %d\n",
                    newsockfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

            // Adds the new socket to array of sockets 
            for (int i = 0; i < max_clients; i++) {  
                // If position is empty 
                if (client_socket[i] == 0) {  
                    client_socket[i] = newsockfd;  
                    client_verified[i] = 0;
                    printf("[+] Adding to list of sockets as %d\n" , i); 
                    break;  
                } 
            }
        }

        // TODO: This is where I/O happens....
        for (int i = 0; i < max_clients; i++){
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, BUFFERSIZE)) == 0){
                    // One of the client has disconnected
                    getpeername(sd, (struct sockaddr*)&cli_addr, &addrsize);
                    printf("[-] A client disconnected, ip: %s, port: %d\n",
                            inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                    client_verified[i] = -1;
                }
                // Incoming message
                else {
                    printf("[.] Reached else and client verfication.\n");
                    if (client_verified[i] == 0){
                        // If the client has not logged in succesfully...
                        printf("[.] Reached if.\n");
                        // Read HTTP request
                        memset(buffer, 0, BUFFERSIZE);
                        printf("[.] Pending valread.\n");
                        valread = read(sd, buffer, BUFFERSIZE);
                        printf("[.] Passed valread.\n");
                        if (valread  < 0) {
                            perror("[-] Error in reading HTTP request.");
                            exit(EXIT_FAILURE);
                        }
                        printf("[.] Reached read\n");
                        printf("%s\n", buffer);
                        char *form = strstr(buffer, "uname=");
                        char response[BUFFERSIZE] = {0};
                        // Display login page
                        if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
                            char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br><button type=\"submit\">Login</button></form></body></html>";
                            sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginHTML), loginHTML);
                            send(newsockfd, response, strlen(response), 0);
                            printf("[.] Reached display\n");
                        }
                        // Extract the username and password from the form data
                        else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
                            char username[MAX_USERNAME_LENGTH] = {0};
                            char password[MAX_PASSWORD_LENGTH] = {0};
                            sscanf(form, "uname=%[^&]&pword=%s", username, password);
                            
                            // User successfully logged in, display question page
                            if (authenticateUsers(username, password)) {
                                char filename[100] = "";
                                strcat(filename, username);
                                strcat(filename, password);
                                strcat(filename, ".csv");

                                // Receive file contents and store into file
                                FILE *fp = fopen(filename, "wb");
                                while ((n = recv(client_socket[i], buffer, BUFFERSIZE, 0)) > 0) {
                                    fwrite(buffer, sizeof(char), n, fp);
                                }
                                printf("[+] File received successfully.\n");
                                fclose(fp);      // Close the file

                                client_verified[i] = 1;
                                char *questionHTML = "<html><body><h1>IN</h1></body></html>";
                                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(questionHTML), questionHTML);
                                send(sd, response, strlen(response), 0);
                            } 
                            // User failed to logged in, ask for login attempt
                            else {
                                char *loginFAILED  = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" required><br><p>Login failed. Try again.</p><br><button type=\"submit\">Login</button></form></body></html>";
                                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginFAILED), loginFAILED);
                                send(sd, response, strlen(response), 0);
                            }
                        } else {
                            // Display 404 error page
                            char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
                            send(sd, errorHTML, strlen(errorHTML), 0);
                        }
                    } else if (client_verified[i] == 1) {
                        // Client has succefully logged in...
                        // loop through questions here...
                    }
                }
            }
        }
    }
}


// #include "TM.h"

// void getUserLogin() {
//     // char *host  = "127.0.0.1";  // host
//     int port    = 8080;         // port
//     int opt     = 1;
//     int sockfd, newsockfd, valread;
//     struct sockaddr_in address;
//     socklen_t addrsize = sizeof(address);
//     char buffer[BUFFERSIZE] = {0};
    
//     // Create socket file descriptor
//     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("[-] Error in socket.");
//         exit(EXIT_FAILURE);
//     }
//     printf("[+] Server socket created.\n");

//     // Set socket options
//     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
//         perror("[-] Error in setting socket options.");
//         exit(EXIT_FAILURE);
//     }
//     printf("[+] Set socket options successful.\n");

//     address.sin_family      = AF_INET;
//     address.sin_port        = htons(port);
//     address.sin_addr.s_addr = INADDR_ANY;

//     // Bind socket to port
//     if (bind(sockfd, (struct sockaddr*)&address, addrsize) < 0) {
//         perror("[-] Error in binding.");
//         exit(EXIT_FAILURE);
//     }
//     printf("[+] Binding successful.\n");

//     // Listen for incoming connections
//     if (listen(sockfd, 10) < 0) {
//         perror("[-] Error in listening.");
//         exit(EXIT_FAILURE);
//     }
//     printf("[+] Listening...\n");

//     // Accept incoming connections
//     while (1) {
//         if ((newsockfd = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrsize)) < 0) {
//             perror("[-] Error in accepting.");
//             exit(EXIT_FAILURE);
//         }

//         // Read HTTP request
//         memset(buffer, 0, BUFFERSIZE);
//         valread = read(newsockfd, buffer, BUFFERSIZE);
//         if (valread < 0) {
//             perror("[-] Error in reading HTTP request.");
//             exit(EXIT_FAILURE);
//         }
//         printf("%s\n", buffer);
//         char *form = strstr(buffer, "uname=");
//         char response[BUFFERSIZE] = {0};

//         // Display login page
//         if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
//             char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br><button type=\"submit\">Login</button></form></body></html>";
//             sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginHTML), loginHTML);
//             send(newsockfd, response, strlen(response), 0);
//         } 
//         // Extract the username and password from the form data
//         else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
//             char username[MAX_USERNAME_LENGTH] = {0};
//             char password[MAX_PASSWORD_LENGTH] = {0};
//             sscanf(form, "uname=%[^&]&pword=%s", username, password);
            
//             // User successfully logged in, display question page
//             if (authenticateUsers(username, password)) {
//                 // Loop for 10 Questions
//                 char *questionHTML = "";
//                 sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(questionHTML), questionHTML);
//                 send(newsockfd, response, strlen(response), 0);
//             } 
//             // User failed to logged in, ask for login attempt
//             else {
//                 char *loginFAILED  = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" required><br><p>Login failed. Try again.</p><br><button type=\"submit\">Login</button></form></body></html>";
//                 sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginFAILED), loginFAILED);
//                 send(newsockfd, response, strlen(response), 0);
//             }
//         } else {
//             // Display 404 error page
//             char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
//             send(newsockfd, errorHTML, strlen(errorHTML), 0);
//         }
//         close(newsockfd);
//     }
// }