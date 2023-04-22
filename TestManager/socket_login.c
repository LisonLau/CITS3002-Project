#include "TM.h"

int opt = 1;
int addrlen = sizeof(address);

void getUserLogin() {
    char buffer[1024] = {0};
    char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br><button type=\"submit\">Login</button></form></body></html>";
    char *loginSUCCESS = "<html><body><h1>Login Successful</h1></body></html>";
    char *loginFAILED  = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" required><br><p>Login failed. Try again.</p><br><button type=\"submit\">Login</button></form></body></html>";
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    handle_socket(new_socket, server_fd, valread, &address, addrlen, buffer, loginHTML, loginSUCCESS, loginFAILED);
}

void handle_socket(int new_socket, int server_fd, int valread, struct sockaddr_in *address, int addrlen, char *buffer, char *loginHTML, char *loginSUCCESS, char *loginFAILED) {
    // Handle incoming connections
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read HTTP request
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);

        // Check if the request is a form submission
        char *form_data = strstr(buffer, "uname=");
        if (form_data != NULL) {
            // Extract the username and password from the form data
            char username[MAX_USERNAME_LENGTH] = {0};
            char password[MAX_PASSWORD_LENGTH] = {0};
            sscanf(form_data, "uname=%[^&]&pword=%s", username, password);
            if (authenticate(username, password)) {
                // Send HTTP response
                char response[1024] = {0};
                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginSUCCESS), loginSUCCESS);
                send(new_socket, response, strlen(response), 0);
            } else {
                // Send HTTP response
                char response[1024] = {0};
                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginFAILED), loginFAILED);
                send(new_socket, response, strlen(response), 0);
            }
        } else {
            // Send the login form HTML
            char response[1024] = {0};
            sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginHTML), loginHTML);
            send(new_socket, response, strlen(response), 0);
        }
        close(new_socket);
    }
}