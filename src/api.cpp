#include "api.hpp"

Api::Api() {}

void Api::initRoutes(crow::App &app) {
    CROW_ROUTE(app, "/api/hello")
    ([](){
        return "Hello, World!";
    });

    CROW_ROUTE(app, "/api/data").methods("POST"_method)([](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400);
        }
        return crow::response(200, "Data received");
    });
}
