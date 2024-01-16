#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = std::stoi(argv[2]);

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

    std::cout << "Connected to the server.\n";

    char command[256];
    while (true) {
        std::cout << "Enter command (SEARCH, RETRIEVE, AVAILABILITY, ORDER, EXIT): ";
        std::cin.getline(command, sizeof(command));

        send(clientSocket, command, strlen(command), 0);

        if (strncmp(command, "EXIT", 4) == 0) {
            std::cout << "Exiting the client.\n";
            break;
        }

        else if (strncmp(command, "AVAILABILITY", 12) == 0) {
    int quantity;
    ssize_t bytesRead = recv(clientSocket, &quantity, sizeof(quantity), 0);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving response from server.\n";
        break;
    }
    std::cout << "Availability: " << quantity << std::endl;
} 
        else {
    char buffer[256];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving response from server.\n";
        break;
    }
    std::cout << "Server Response:\n" << buffer << std::endl;
}

    }

    close(clientSocket);

    return 0;
}
