#include <iostream>
#include <cstring>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
        return 1;
    }
    const char* ip=argv[1];
    int port = std::stoi(argv[2]);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 2) == -1) {
        perror("Error listening for connections");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for connections...\n";

    int conn1 = accept(serverSocket, nullptr, nullptr);
    std::cout << "Client1 connected.\n";

    int conn2 = accept(serverSocket, nullptr, nullptr);
    std::cout << "Client2 connected.\n";

    float charFromClient1;
    recv(conn1, &charFromClient1, sizeof(charFromClient1), 0);

    float charToClient2 = std::pow(charFromClient1,1.5);
    charToClient2 = std::round(charToClient2 * 100) / 100;

    std::cout << "Received from Client1: " << charFromClient1 << std::endl;
    std::cout << "Sending to Client2: " << charToClient2 << std::endl;

    send(conn2, &charToClient2, sizeof(charToClient2), 0);

    close(conn1);
    close(conn2);
    close(serverSocket);

    return 0;
}

