#include "http_server.hpp"
#include "logging.hpp"
#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

HTTPServer::HTTPServer(int port) : port(port), serverSocket(0) {
    initSocket();
}

HTTPServer::~HTTPServer() {
    close(serverSocket);
}

void HTTPServer::initSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        INFO("Failed to create socket");
        exit(1);
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        INFO("Failed to bind port");
        exit(1);
    }

    if (listen(serverSocket, 10) < 0) {
        INFO("Failed to listen on port");
        exit(1);
    }
}

void HTTPServer::start() {
    INFO("Server started!");

    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    while (true) {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            INFO("Failed to accept incoming client request! HAHA!!!");
            continue;
        }
        handleClient(clientSocket);
        close(clientSocket);
    }
}

void HTTPServer::handleClient(int clientSocket) {
    char buffer[1024];
    std::memset(buffer, 0, sizeof(buffer));

    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead < 0) {
        INFO("Failed to read socket client");
        return;
    }

    INFO("Recieved request");
    std::cout << buffer << std::endl;

    std::string responseContent = "<html><body><h1>if this comes up im gonna explode myself</h1></body></html>";
    sendResponse(clientSocket, responseContent);
}

void HTTPServer::sendResponse(int clientSocket, const std::string& content) {
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << content;

    std::string responseStr = response.str();
    write(clientSocket, responseStr.c_str(), responseStr.size());
}
