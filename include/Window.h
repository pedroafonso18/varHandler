#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../lib/imgui/backends/imgui_impl_sdl2.h"
#include "../lib/imgui/backends/imgui_impl_opengl3.h"
#include "../lib/imgui/misc/cpp/imgui_stdlib.h"

#include "imgui.h"

class Window {
private:
    SDL_Window* window;
    SDL_GLContext gl_context;
    ImGuiIO* io;
    int StartSDL();
    int StartImGui();
public:
    Window();
    ~Window();

    int Initialize();
    bool ProcessEvents();
    void NewFrame();
    void Render();

    SDL_Window* GetWindow() const;
    SDL_GLContext GetGLContext() const;
};
