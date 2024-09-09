#pragma once

#include <string>
#include "UnityEngine/GameObject.hpp"

std::string getHostIP(const std::string& host);

std::string fetchHTML(const std::string& host, const std::string& page);

bool parseChatMessage(const std::string& html, std::string& username, std::string& message);

void continuouslyCheckForMessages(const std::string& videoID, void (*onNewMessage)(const std::string&, const std::string&));
