cmake_minimum_required(VERSION 3.13)
project(skifree_wasm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten specific settings
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/obstacle.cpp
)

# Include directories
include_directories(include)

# Create executable
add_executable(skifree ${SOURCES})

target_link_libraries(skifree SDL2) 