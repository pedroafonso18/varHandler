#define SDL_MAIN_HANDLED
#include <iostream>
#include "../include/Config.h"
#include "../include/Database.h"
#include "../include/Logger.h"
#include <SDL2/SDL.h>
#include "../lib/imgui/imgui.h"
#include <GL/glew.h>
#include "../lib/imgui/backends/imgui_impl_sdl2.h"
#include "../lib/imgui/backends/imgui_impl_opengl3.h"
#include "../lib/imgui/misc/cpp/imgui_stdlib.h"

Logger apiLogger("../logs/api.log");

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init ERROR: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("Var Handler",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "SDL_CreateWindow ERROR: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "SDL_GL_CreateContext ERROR: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // this enables vsync, apparently.

    if (glewInit() != 0) {
        std::cerr << "glewInit ERROR: Couldn't initialize OPENGL Loader!\n";
        return 1;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    Config cfg;
    const auto& env = cfg.getEnv();

    Database db(env.db_url);
    auto vars = db.fetchVars();

    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Database Variables");
        for (const auto& var : vars) {
            ImGui::Text("%s: %s", var.first.c_str(), var.second.c_str());
        }
        ImGui::End();
        ImGui::Begin("Insert New Var");
        static std::string key;
        static std::string value;

        ImGui::InputText("New Var Key", &key);
        ImGui::InputText("New Var Value", &value);

        if (ImGui::Button("Send"))
        {
            printf("Key: %s | Value: %s\n", key.c_str(), value.c_str());

            key.clear();
            value.clear();
        }

        ImGui::End();
        ImGui::Render();
        glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
