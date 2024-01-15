#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void sendCommand(const char* command, int serverSocket) {
    if (send(serverSocket, command, strlen(command), 0) == -1) {
        perror("Error sending command to server");
        close(serverSocket);
        exit(1);
    }
}

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

    while (true) {
        std::cout << "Enter a command ('EXIT' to exit): ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "EXIT" || command == "exit") {
            break;
        }

        // Send command to the server
        sendCommand(command.c_str(), clientSocket);

        // Receive and print the server's response
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Server response:\n" << buffer << std::endl;
        } else {
            std::cerr << "Error receiving response from server.\n";
        }
    }

    close(clientSocket);

    return 0;
}

