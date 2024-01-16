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
        perror("Error connecting to the server");
        close(clientSocket);
        return 1;
    }

    std::cout << "Connected to the server.\n";

    // Accept user commands until the user enters "EXIT"
    std::string command;
    while (true) {
        std::cout << "Enter command (or type EXIT to exit): ";
        std::getline(std::cin, command);

        if (command == "EXIT") {
            break;
        }

        // Send the command to the server
        send(clientSocket, command.c_str(), command.size(), 0);

        // Receive and print the server's response
        char buffer[256];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving response from the server.\n";
            break;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the received data
        std::cout << "Server Response:\n" << buffer << std::endl;
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}
