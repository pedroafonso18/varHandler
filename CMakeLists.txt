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
)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")

target_include_directories(varHandler PRIVATE ${CMAKE_SOURCE_DIR}/lib/spdlog)

target_include_directories(varHandler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src
)

link_directories("C:/msys64/mingw64/lib")
target_link_libraries(varHandler PRIVATE pqxx)