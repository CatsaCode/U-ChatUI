#pragma once

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

void start_server(int port);

std::string read_html_file(const std::string& filename);

void append_to_html(std::string& html, const std::string& custom_content);
