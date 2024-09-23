#pragma once
#ifndef API_HPP
#define API_HPP

#include <crow.h>

class Api {
public:
    Api();
    void initRoutes(crow::App &app);
};

#endif
