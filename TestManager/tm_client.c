#include "TM.h"
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>

int connectToQb(char *username, char *password){
	int n, cli_sockfd;
	struct sockaddr_in server_addr;
    char buffer[BUFFERSIZE] = {0};
	// char message[1000] , server_reply[2000];
    socklen_t serverlen = sizeof(server_addr);
	
	//Create socket
	cli_sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if (cli_sockfd < 0) {
		printf("[-] Failed to create QB client socket");
	}
	puts("Socket created");
	
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( 8888 );

	// Connect to remote server
	if (connect(cli_sockfd , (struct sockaddr *)&server_addr , serverlen) < 0) {
		perror("[-] Connection to QB server failed. Error");
		return 1;
	}
	printf("[+] New connection to the QB server...\n");
	// puts("Connected\n");
	
	// keep communicating with server
	while(1) {
        // // TODO: Will likely need to send over the login first to get the questions.
		// printf("Enter message : ");
		// scanf("%s" , buffer);

        // //Send some data
		// if(send(cli_sockfd , buffer, BUFFERSIZE, 0) < 0) {
		// 	puts("[-] Send failed");
		// 	return 1;
		// }

        memset(buffer, 0, BUFFERSIZE);
        char filename[100] = "";
        strcat(filename, username);
        strcat(filename, password);
        strcat(filename, ".csv");

        // Receive file contents from QB and store into file
        FILE *fp = fopen(filename, "wb");
        while ((n = recv(cli_sockfd, buffer, BUFFERSIZE, 0)) > 0) {
            fwrite(buffer, sizeof(char), n, fp);
        }
        printf("[+] File received successfully.\n");
        fclose(fp);      // Close the file
		break;
		// puts("[.] Server reply :");
		// puts(buffer);
	}
	
	close(cli_sockfd);
    return 0;
}