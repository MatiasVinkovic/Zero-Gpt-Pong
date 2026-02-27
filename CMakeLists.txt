cmake_minimum_required(VERSION 3.14)
project(ZeroGptPong VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── SDL2 ────────────────────────────────────────────────────────────────────
find_package(SDL2 REQUIRED)

# SDL2_mixer may not ship a CMake config; fall back to pkg-config / manual search
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
    PATH_SUFFIXES SDL2
    PATHS /usr/include /usr/local/include)

find_library(SDL2_MIXER_LIBRARY
    NAMES SDL2_mixer
    PATHS /usr/lib /usr/lib/x86_64-linux-gnu /usr/local/lib)

if(NOT SDL2_MIXER_INCLUDE_DIR OR NOT SDL2_MIXER_LIBRARY)
    message(FATAL_ERROR "SDL2_mixer not found. Install libsdl2-mixer-dev.")
endif()

# ── Sources ──────────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Game.cpp
)

add_executable(pong ${SOURCES})

target_include_directories(pong PRIVATE
    src
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIR})

target_link_libraries(pong PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_MIXER_LIBRARY}
    m)
