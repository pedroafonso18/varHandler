#define SDL_MAIN_HANDLED
#include <iostream>
#include <thread>
#include "../include/Config.h"
#include "../include/Database.h"
#include "../include/Logger.h"
#include "../include/Window.h"
#include <SDL2/SDL.h>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/misc/cpp/imgui_stdlib.h"
#include <unordered_map>
#include "../lib/crow_all.h"
#include <atomic>
#include <sstream>
#include <future>

Logger apiLogger("../logs/api.log");
std::atomic<bool> shouldExit(false);

void runApiServer(Database& db) {
    crow::SimpleApp app;

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

int main(int argc, char* argv[]) {
    Window window;
    if (window.Initialize() != 0) {
        return 1;
    }

    Config cfg;
    const auto& env = cfg.getEnv();

    Database db(env.db_url);

    std::thread apiThread(runApiServer, std::ref(db));

    bool running = true;
    static std::unordered_map<std::string, std::string> newValues;

    while (running) {
        running = window.ProcessEvents();

        window.NewFrame();
        auto vars = db.fetchVars();

        ImGui::Begin("Database Variables");
        for (const auto& var : vars) {
            ImGui::Text("%s: %s", var.first.c_str(), var.second.c_str());
            if (ImGui::SmallButton((std::string("EDIT##") + var.first).c_str())) {
                newValues[var.first] = var.second;
            }
            if (ImGui::SmallButton((std::string("DELETE##")+ var.first).c_str())) {
                db.deleteOperation(var.first);
            }
            if (newValues.contains(var.first)) {
                ImGui::InputText((std::string("New value##") + var.first).c_str(), &newValues[var.first]);
                if (ImGui::SmallButton((std::string("SEND##") + var.first).c_str())) {
                    db.updateOperation(std::make_pair(var.first, newValues[var.first]));
                    newValues.erase(var.first);
                }
            }
        }
        ImGui::End();
        ImGui::Begin("Insert New Var");
        static std::string key;
        static std::string value;

        ImGui::InputText("Key", &key);
        ImGui::InputText("Value", &value);

        if (ImGui::Button("Send"))
        {
            printf("Key: %s | Value: %s\n", key.c_str(), value.c_str());
            db.saveOperation(std::make_pair(key, value));
            key.clear();
            value.clear();
        }

        ImGui::Separator();
        ImGui::Text("API server running on port 8080");

        ImGui::End();

        window.Render();
    }

    shouldExit.store(true);
    apiThread.join();

    return 0;
}
