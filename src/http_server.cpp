#include "http_server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

HTTPServer::HTTPServer(int port) : port(port), serverSocket(0) {
    initSocket();
}

HTTPServer::~HTTPServer() {
    if (serverSocket > 0) {
        close(serverSocket);
    }
}

void HTTPServer::initSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error: Failed to create socket." << std::endl;
        exit(1);
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error: Failed to set socket options." << std::endl;
        close(serverSocket);
        exit(1);
    }

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Failed to bind to port " << port << std::endl;
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error: Failed to listen on port " << port << std::endl;
        close(serverSocket);
        exit(1);
    }

    std::cout << "Server initialized on port " << port << std::endl;
}

void HTTPServer::start() {
    std::cout << "Server started, waiting for connections on port " << port << std::endl;

    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    while (true) {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "Error: Failed to accept client connection." << std::endl;
            continue;
        }

        std::cout << "Connection accepted from "
                  << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        handleClient(clientSocket);
        close(clientSocket);
    }
}

void HTTPServer::handleClient(int clientSocket) {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead < 0) {
        std::cerr << "Error: Failed to read from client socket." << std::endl;
        return;
    }

    std::cout << "Received request:\n" << buffer << std::endl;

    std::string responseContent = "<html><body><h1>pls work!</h1></body></html>";
    sendResponse(clientSocket, responseContent);
}

void HTTPServer::sendResponse(int clientSocket, const std::string& content) {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
    response += "Content-Type: text/html\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += content;

    write(clientSocket, response.c_str(), response.size());
}
