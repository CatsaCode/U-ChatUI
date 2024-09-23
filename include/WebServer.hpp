#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <httplib.h>
#include <string>
#include <json/json.h>
#include <iostream>

namespace WebServer {
    void start();
}

httplib::Server server;

void handleGetRequest(const httplib::Request& req, httplib::Response& res);
void handlePostRequest(const httplib::Request& req, httplib::Response& res);



#endif // WEBSERVER_HPP