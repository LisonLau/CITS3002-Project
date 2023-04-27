#include "TM.h"

void runTMserver() {
    int opt     = 1;
    int max_clients = 30;
    int sd, max_sd, activity, valread;
    int ser_sockfd, newsockfd, client_socket[30], client_verified[30];
    struct sockaddr_in address;
    socklen_t addrsize = sizeof(address);
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

    // Clear the address struct
    memset((char *) &address, 0, sizeof(address));

    // Set socket options
    if (setsockopt(ser_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    address.sin_family      = AF_INET;
    address.sin_port        = htons(PORT);
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;

    // Bind socket to port
    if (bind(ser_sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
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
            sd = client_socket[i];
            if (sd > 0){
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd){
                max_sd = sd;
            }
        } 

        // Waiting for one of the sockets to do something, waits indefinitel
        printf("[+] Waiting...\n");
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0 && errno != EINTR){
            printf("[-] Socket connection error.\n");
        }

        // When something happens on the server socket = incoming connection
        if (FD_ISSET(ser_sockfd, &readfds)){
            if ((newsockfd = accept(ser_sockfd, (struct sockaddr*)&address, &addrsize)) < 0) {
                perror("[-] Error in accepting connection: ");
                exit(EXIT_FAILURE);
            }
            printf("[+] New connection, socket fd: %d, ip: %s, port: %d\n",
                    newsockfd, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

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
                memset(buffer, 0, BUFFERSIZE);
                valread = read(sd, buffer, BUFFERSIZE);
                printf("[+] valread: %i\n", valread);
                if (valread == 0){
                    // One of the client has disconnected
                    client_socket[i] = 0;
                    client_verified[i] = -1;
                    getpeername(sd, (struct sockaddr*)&address, &addrsize);
                    printf("[-] A client disconnected, ip: %s, port: %d\n",
                            inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    close(sd);
                }
                // Incoming message (client is still connected)
                else {
                    if (client_verified[i] == 0){
                        // If the client has not logged in succesfully...
                        // Read HTTP request
                        printf("[.]\n%s[.]\n", buffer);
                        char *form = strstr(buffer, "uname=");
                        char response[BUFFERSIZE] = {0};

                        // Display login page
                        if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
                            char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br><button type=\"submit\">Login</button></form></body></html>";
                            sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginHTML), loginHTML);
                            send(sd, response, strlen(response), 0);
                        } 
                        // Extract the username and password from the form data
                        else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
                            char username[MAX_USERNAME_LENGTH] = {0};
                            char password[MAX_PASSWORD_LENGTH] = {0};
                            sscanf(form, "uname=%[^&]&pword=%s", username, password);
                            
                            // User successfully logged in, display question page
                            if (authenticateUsers(username, password)) {
                                client_verified[i] = 1;
                                int passed = connectToQB(username, password);

                                printf("[.] Exchange with QB: %i\n", passed);
                                char filename[100] = "";
                                strcat(filename, username);
                                strcat(filename, password);
                                strcat(filename, ".csv");

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
                    } if (client_verified[i] == 1) {
                        // Client has succefully logged in...
                        // loop through questions here...
                        printf("[+] User has been verified!\n");
                    }
                }
            }
        }
    }
}