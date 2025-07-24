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
#include "../include/Api.h"
#include <atomic>
#include <sstream>
#include <future>

Logger apiLogger("../logs/api.log");


int main(int argc, char* argv[]) {
    Window window;
    if (window.Initialize() != 0) {
        return 1;
    }

    Config cfg;
    const auto& env = cfg.getEnv();

    Database db(env.db_url);
    Api api;

    std::thread apiThread(&Api::runApiServer, &api, std::ref(db), env.apikey);

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
