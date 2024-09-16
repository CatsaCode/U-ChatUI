#include "server.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

std::string read_html_file(const std::string& filename) {
    std::ifstream file(filename);

    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

void append_to_html(std::string& html, const std::string& custom_content) {
    size_t pos = html.find("</body>");
    if (pos != std::string::npos) {
        html.insert(pos, custom_content);
    }
}

void handle_request(int client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    read(client_socket, buffer, sizeof(buffer) - 1);

    std::string request(buffer);
    if (request.find("GET / ") != std::string::npos || request.find("GET /index.html ") != std::string::npos) {
        std::string html_content = read_html_file("index.html");

        std::string custom_content = "<p>If I did everything right, this should show up, right :D</p>";
        append_to_html(html_content, custom_content);


        // Stealed that from an website because I don't know anything about that HTML shit
        std::string http_response = "HTTP/1.1 200 OK\r\n";
        http_response += "Content-Type: text/html\r\n";
        http_response += "Content-Length: " + std::to_string(html_content.size()) + "\r\n";
        http_response += "Connection: close\r\n";
        http_response += "\r\n";
        http_response += html_content;

        send(client_socket, http_response.c_str(), http_response.size(), 0);
    } else {
        std::string not_found_response = "HTTP/1.1 404 Not Found\r\n";
        not_found_response += "Content-Type: text/html\r\n";
        not_found_response += "Content-Length: 13\r\n";
        not_found_response += "Connection: close\r\n";
        not_found_response += "\r\n";
        not_found_response += "404 Not Found";

        send(client_socket, not_found_response.c_str(), not_found_response.size(), 0);
    }

    close(client_socket);
}

void start_server(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);

        handle_request(client_socket);
    }

    close(server_socket);
}