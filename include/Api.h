#pragma once
#include "../include/Database.h"
#include "../lib/crow_all.h"
#include "../include/Logger.h"
#include <atomic>
#include <string>

extern std::atomic<bool> shouldExit;

class Api {
private:
    std::string apikey;

public:
    Api(const std::string& key = "ABC123") : apikey(key) {}

    struct Middleware {
        Api* api;
        Middleware(Api* apiPtr) : api(apiPtr) {}

        struct context {};

        void before_handle(crow::request& req, crow::response& res, context& ctx) {
            if (req.get_header_value("apikey") != api->getApiKey()) {
                res.code = 403;
                res.end();
            }
        }

        void after_handle(crow::request& req, crow::response& res, context& ctx)
        {}
    };

    void runApiServer(Database& db, std::string apikey);
    void setApiKey(const std::string& key) { apikey = key; }
    std::string getApiKey() const { return apikey; }
};
