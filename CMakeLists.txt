cmake_minimum_required(VERSION 3.20)
project(chen_emily_project3_Spring2026)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML REQUIRED COMPONENTS Graphics Window System)

add_executable(chen_emily_project3_Spring2026 src/main.cpp
        src/main.cpp
        src/WelcomeWindow.cpp
        src/WelcomeWindow.h
        src/board.cpp
        src/board.h
        src/Tiles.h
        src/Tiles.cpp
        src/Leaderboard.h
        src/Leaderboard.cpp)
target_compile_features(chen_emily_project3_Spring2026 PRIVATE cxx_std_17)
target_link_libraries(chen_emily_project3_Spring2026 PRIVATE SFML::Graphics SFML::Window SFML::System)