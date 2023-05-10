#include "TM.h"

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        perror("Usage:\t./TM clientNetworkIP\n");
        exit(EXIT_FAILURE);
    }
    clientIpAddr = argv[1];
    storeUsers();
    runTMforWeb();
}
