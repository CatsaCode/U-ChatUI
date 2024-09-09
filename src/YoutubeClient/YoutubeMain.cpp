
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <chrono>
#include <thread>

#include "YoutubeClient/Socket.hpp"
#include "custom-types/shared/register.hpp"

std::string getHostIP(const std::string& host) {
    struct hostent* he = gethostbyname(host.c_str());
    if (he == nullptr) {
        return "";
    }
    struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;
    return inet_ntoa(*addr_list[0]);
}

std::string fetchHTML(const std::string& host, const std::string& page) {
    std::string ip = getHostIP(host);
    if (ip.empty()) return "";

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return "";
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        close(sock);
        return "";
    }

    std::string request = "GET " + page + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";

    send(sock, request.c_str(), request.length(), 0);

    char buffer[10000];
    std::string html;
    int bytesReceived = 0;

    while ((bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        html += buffer;
    }

    close(sock);

    return html;
}

bool parseChatMessage(const std::string& html, std::string& username, std::string& message) {
    std::string marker = "live-chat-text-message";
    size_t pos = html.find(marker);
    if (pos == std::string::npos) {
        return false;
    }

    std::string chatSnippet = html.substr(pos, 1000);

    std::string userStart = "author-name\">";
    std::string userEnd = "</span>";
    size_t userPosStart = chatSnippet.find(userStart);
    size_t userPosEnd = chatSnippet.find(userEnd, userPosStart);
    
    if (userPosStart != std::string::npos && userPosEnd != std::string::npos) {
        userPosStart += userStart.length();
        username = chatSnippet.substr(userPosStart, userPosEnd - userPosStart);
    } else {
        username = "Unknown";
    }

    std::string msgStart = "message\">";
    std::string msgEnd = "</span>";
    size_t msgPosStart = chatSnippet.find(msgStart);
    size_t msgPosEnd = chatSnippet.find(msgEnd, msgPosStart);

    if (msgPosStart != std::string::npos && msgPosEnd != std::string::npos) {
        msgPosStart += msgStart.length();
        message = chatSnippet.substr(msgPosStart, msgPosEnd - msgPosStart);
        return true;
    } else {
        message = "Message not found";
        return false;
    }
}

void continuouslyCheckForMessages(const std::string& videoID, void (*onNewMessage)(const std::string&, const std::string&)) {
    std::string lastMessage = "";

    while (true) {
        std::string host = "www.youtube.com";
        std::string page = "/live_chat?v=" + videoID;

        std::string html = fetchHTML(host, page);

        if (!html.empty()) {
            std::string username;
            std::string message;
            if (parseChatMessage(html, username, message)) {
                if (message != lastMessage) {
                    lastMessage = message;
                    onNewMessage(username, message);
                }
            }
        } else {
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
