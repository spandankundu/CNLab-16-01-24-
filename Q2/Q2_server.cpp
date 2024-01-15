#include <iostream>
#include <unordered_map>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

struct Part {
    std::string partName;
    float partPrice;
    int partQuantity;
    std::string partDescription;
};

std::unordered_map<int, Part> inventory;

void searchPart(int partNumber, int clientSocket) {
    // Implement search logic based on specified criteria
    // For demonstration, just printing part details
    if (inventory.find(partNumber) != inventory.end()) {
        std::string result = "Part details for part number " + std::to_string(partNumber) + ":\n";
        result += "Name: " + inventory[partNumber].partName + "\n";
        result += "Price: " + std::to_string(inventory[partNumber].partPrice) + "\n";
        result += "Quantity: " + std::to_string(inventory[partNumber].partQuantity) + "\n";
        result += "Description: " + inventory[partNumber].partDescription + "\n";
        send(clientSocket, result.c_str(), result.size(), 0);
    } else {
        std::string notFoundMsg = "Part with part number " + std::to_string(partNumber) + " not found.\n";
        send(clientSocket, notFoundMsg.c_str(), notFoundMsg.size(), 0);
    }
}

std::string retrievePartName(int partNumber) {
    // Implement logic to retrieve part name for a given part number
    // For demonstration, returning a placeholder name
    return "Sample Part";
}

int getPartQuantity(int partNumber) {
    // Implement logic to get the quantity of available parts
    // For demonstration, returning a placeholder quantity
    return 10;
}

void orderPart(int partNumber) {
    // Implement logic for ordering a part
    // For demonstration, just decrementing the quantity
    if (inventory.find(partNumber) != inventory.end() && inventory[partNumber].partQuantity > 0) {
        inventory[partNumber].partQuantity--;
        std::cout << "Ordered part with part number " << partNumber << std::endl;
    } else {
        std::cout << "Part with part number " << partNumber << " not available." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip> <port>\n";
        return 1;
    }

    const char* ip = argv[1];
    int port = std::stoi(argv[2]);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
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

    std::cout << "Server is listening for connections on port " << port << "...\n";

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        std::cout << "Client connected.\n";

        // Receive command from the client
        char buffer[256];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error receiving command from client.\n";
            close(clientSocket);
            continue;
        }

        // Parse the command and execute corresponding server-side procedure
        std::string command(buffer, bytesRead);
        int partNumber;

        if (command.substr(0, 6) == "SEARCH") {
            sscanf(buffer, "SEARCH %d", &partNumber);
            searchPart(partNumber, clientSocket);
        } else if (command.substr(0, 10) == "RETRIEVE") {
            sscanf(buffer, "RETRIEVE %d", &partNumber);
            std::string partName = retrievePartName(partNumber);
            send(clientSocket, partName.c_str(), partName.size(), 0);
        } else if (command.substr(0, 11) == "AVAILABILITY") {
            sscanf(buffer, "AVAILABILITY %d", &partNumber);
            int quantity = getPartQuantity(partNumber);
            send(clientSocket, &quantity, sizeof(quantity), 0);
        } else if (command.substr(0, 5) == "ORDER") {
            sscanf(buffer, "ORDER %d", &partNumber);
            orderPart(partNumber);
        } else {
            std::cerr << "Invalid command received.\n";
        }

        close(clientSocket);  // Close the connection with the client
    }

    close(serverSocket);

    return 0;
}

