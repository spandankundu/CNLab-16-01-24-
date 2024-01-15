#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port> <character>\n";
        return 1;
    }

    const char *ip = argv[1];
    int port = std::stoi(argv[2]);
    int charToSend = std::stoi(argv[3]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return 1;
    }

    send(clientSocket, &charToSend, sizeof(charToSend), 0);

    close(clientSocket);

    return 0;
}

