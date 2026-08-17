cmake_minimum_required(VERSION 3.19)
project(FreshmanJourney VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
    src/Game.cpp
    src/Map.cpp
    src/Player.cpp
    src/Menu.cpp
    src/SaveManager.cpp
    src/TimeSystem.cpp
    src/Utility.cpp
)

include_directories(include)
add_executable(FreshmanJourney ${SOURCE_FILES})
