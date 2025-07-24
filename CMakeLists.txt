cmake_minimum_required(VERSION 3.31)
project(varHandler)

set(CMAKE_CXX_STANDARD 20)


add_executable(varHandler
        src/main.cpp
        include/database.h
        src/Database.cpp
        include/Logger.h
        src/Logger.cpp
        include/Config.h
        src/Config.cpp
        lib/dotenv.h
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_tables.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_sdl2.cpp
        lib/imgui/backends/imgui_impl_opengl3.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
        lib/crow_all.h
        include/Window.h
        src/Window.cpp
)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")

target_include_directories(varHandler PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(varHandler PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(varHandler PRIVATE ${CMAKE_SOURCE_DIR}/lib/imgui)
target_include_directories(varHandler PRIVATE "C:/msys64/mingw64/include/SDL2")
target_include_directories(varHandler PRIVATE "C:/msys64/mingw64/include/asio")

target_include_directories(varHandler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

link_directories("C:/msys64/mingw64/lib")
target_link_libraries(varHandler PRIVATE pqxx SDL2 SDL2main OpenGL32 glew32 ws2_32 mswsock)
