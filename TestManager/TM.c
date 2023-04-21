#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_STUDENTS        100
#define BUFFERSIZE          1024

typedef struct Student {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Student;

Student students[MAX_STUDENTS];
int     numStudents;

// Function to store registered student usernames and passwords
void storeRegistered() {
    FILE *fp;
    char line[100];
    char *token;
    int count = 0;

    fp = fopen("students.csv", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
    }
 
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Parse the line using strtok
        token = strtok(line, ",");
        if (token != NULL) {
            strncpy(students[count].username, token, MAX_USERNAME_LENGTH - 1);
            students[count].username[MAX_USERNAME_LENGTH - 1] = '\0';
        }
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(students[count].password, token, MAX_PASSWORD_LENGTH - 1);
            students[count].password[strcspn(students[count].password, "\r\n")] = 0;
        }
        count++;
    }

    fclose(fp);
    numStudents = count;
}

int authenticate(char *username, char *password) {
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(username, students[i].username) == 0 && strcmp(password, students[i].password) == 0) {
            return 1; 
        }
    }
    return 0;
}

char* createQuestionHTML() {
    char *questionHTML = "";
}

void getUserLogin() {
    char *host  = "127.0.0.1";  // host
    int port    = 8080;         // port
    int opt     = 1;
    int sockfd, newsockfd, valread;
    struct sockaddr_in address;
    socklen_t addrsize = sizeof(address);
    char buffer[BUFFERSIZE] = {0};
    
    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error in socket.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Server socket created.\n");

    // Set socket options
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[-] Error in setting socket options.");
        exit(EXIT_FAILURE);
    }
    printf("[+] Set socket options successful.\n");

    address.sin_family      = AF_INET;
    address.sin_port        = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(sockfd, (struct sockaddr*)&address, addrsize) < 0) {
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
    while (1) {
        if ((newsockfd = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrsize)) < 0) {
            perror("[-] Error in accepting.");
            exit(EXIT_FAILURE);
        }

        // Read HTTP request
        memset(buffer, 0, BUFFERSIZE);
        valread = read(newsockfd, buffer, BUFFERSIZE);
        printf("%s\n", buffer);
        char *form = strstr(buffer, "uname=");
        char response[BUFFERSIZE] = {0};

        if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
            // Display login page
            char *loginHTML = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" value=\"\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" value=\"\" required><br><br><button type=\"submit\">Login</button></form></body></html>";
            sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginHTML), loginHTML);
            send(newsockfd, response, strlen(response), 0);
        } else if (strstr(buffer, "POST / HTTP/1.1") != NULL) {
            // Extract the username and password from the form data
            char username[MAX_USERNAME_LENGTH] = {0};
            char password[MAX_PASSWORD_LENGTH] = {0};
            sscanf(form, "uname=%[^&]&pword=%s", username, password);
            if (authenticate(username, password)) {
                // Display question page
                char *questionHTML = createQuestionHTML();
                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(questionHTML), questionHTML);
                send(newsockfd, response, strlen(response), 0);
            } else {
                // Ask for relogin
                char *loginFAILED  = "<html><body><h1>Login</h1><form method=\"post\"><label for=\"uname\">Username : </label><input type=\"text\" name=\"uname\" required><br><br><label for=\"pword\">Password : </label><input type=\"text\" name=\"pword\" required><br><p>Login failed. Try again.</p><br><button type=\"submit\">Login</button></form></body></html>";
                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: %ld\n\n%s", strlen(loginFAILED), loginFAILED);
                send(newsockfd, response, strlen(response), 0);
            }
        } else {
            // Display 404 error page
            char* errorHTML = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>404 Not Found</title>\n</head>\n<body>\n<h1>404 Not Found</h1>\n<p>The requested URL was not found on this server.</p>\n</body>\n</html>";
            send(newsockfd, errorHTML, strlen(errorHTML), 0);
        }
        close(newsockfd);
    }
}

int main(int argc, char const *argv[]) {
    storeRegistered();
    getUserLogin();

}
