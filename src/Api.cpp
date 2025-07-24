#include "../include/Api.h"

extern Logger apiLogger;
std::atomic<bool> shouldExit(false);


void Api::runApiServer(Database& db, std::string apikey) {
    this->apikey = apikey;
    Middleware middleware(this);
    crow::App<Middleware> app(std::move(middleware));

    CROW_ROUTE(app, "/getVars")
    .methods("GET"_method)
    ([&db]() {
        crow::json::wvalue result;
        auto vars = db.fetchVars();
        for (const auto& var : vars) {
            result[var.first] = var.second;
        }
        return crow::response(result);
    });

    CROW_ROUTE(app, "/getVar/<string>")
    .methods("GET"_method)
    ([&db](const std::string& key) {
        auto vars = db.fetchVars();
        if (vars.find(key) != vars.end()) {
            return crow::response(vars[key]);
        } else {
            return crow::response(404, "Variable not found");
        }
    });

    CROW_ROUTE(app, "/setVar")
    .methods("POST"_method)
    ([&db](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x || !x.has("key") || !x.has("value"))
            return crow::response(crow::status::BAD_REQUEST);

        std::string key = x["key"].s();
        std::string value = x["value"].s();

        db.saveOperation(std::make_pair(key, value));
        apiLogger.info("API: Set variable " + key + " to " + value);

        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/updateVar")
    .methods("PUT"_method)
    ([&db](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x || !x.has("key") || !x.has("value"))
            return crow::response(crow::status::BAD_REQUEST);

        std::string key = x["key"].s();
        std::string value = x["value"].s();

        db.updateOperation(std::make_pair(key, value));
        apiLogger.info("API: Updated variable " + key + " to " + value);

        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/deleteVar/<string>")
    .methods("DELETE"_method)
    ([&db](const std::string& key) {
        db.deleteOperation(key);
        apiLogger.info("API: Deleted variable " + key);
        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/shutdown")
    .methods("POST"_method)
    ([&]() {
        shouldExit.store(true);
        return crow::response(crow::status::OK);
    });

    app.port(8080).multithreaded();

    std::cout << "API server running on port 8080" << std::endl;
    apiLogger.info("API server started on port 8080");

    auto future = std::async(std::launch::async, [&] {
        app.run();
    });

    while (!shouldExit.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    app.stop();
    apiLogger.info("API server shutting down");
}
