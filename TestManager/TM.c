// Student 1: Allison Lau   (23123849)
// Student 2: Alicia Lau    (22955092)
// Student 3: Li-Anne Long  (23192171)

#include "TM.h"

/**
 * @brief Running the main program for TM
 * @param argc number of command-line arguments provided to the program
 * @param argv array of strings representing the command-line arguments
 * @return exit status of the program 
 */
int main(int argc, char const *argv[]) {
    // Check if there are two arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <TM IP address> <QB IP address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct in_addr ip1, ip2;
    // Check if the first argument is a valid IP address
    SERVER_HOST = argv[1];
    if (inet_pton(AF_INET, SERVER_HOST, &ip1) != 1) {
        fprintf(stderr, "Error: Invalid TM IP address '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Check if the second argument is a valid IP address
    CLIENT_HOST = argv[2];
    if (inet_pton(AF_INET, CLIENT_HOST, &ip2) != 1) {
        fprintf(stderr, "Error: Invalid QB IP address '%s'\n", argv[2]);
        return EXIT_FAILURE;
    }
    
    storeUsers();   // Store registered users
    runTMforWeb();  // Run the TM server for web browser clients
    exit(EXIT_SUCCESS);
}
