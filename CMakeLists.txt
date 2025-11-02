cmake_minimum_required(VERSION 3.14)
project(Orbitbackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

add_executable(orbit
    src/main.cpp
)
find_package(Crow CONFIG REQUIRED)
target_link_libraries(orbit PRIVATE Crow::Crow)
find_package(Threads REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(orbit PRIVATE unofficial::sqlite3::sqlite3)

target_link_libraries(orbit PRIVATE Threads::Threads)
