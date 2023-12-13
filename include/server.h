#pragma once

#include <crow.h>
#include <config.h>

class Server
{
public:
    Server(const Config& config);
    void register_handlers(crow::SimpleApp& app);
private:
    crow::response get_index(crow::mustache::context ctx);
    crow::response upload(const crow::request& request);
private:
    const Config& config;
};