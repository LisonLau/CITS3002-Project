#include "TM.h"

int main(int argc, char const *argv[]) {
    // Check if there are two arguments
    if (argc != 3) {
        printf("Usage: %s <TM IP address> <QB IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct in_addr ip1, ip2;

    // Check if the first argument is a valid IPv4 address
    SERVER_HOST = argv[1];
    if (inet_pton(AF_INET, SERVER_HOST, &ip1) != 1) {
        printf("Error: Invalid TM IP address '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Check if the second argument is a valid IPv4 address
    CLIENT_HOST = argv[2];
    if (inet_pton(AF_INET, CLIENT_HOST, &ip2) != 1) {
        printf("Error: Invalid QB IP address '%s'\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    storeUsers();
    runTMforWeb();
    
}
