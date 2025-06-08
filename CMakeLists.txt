cmake_minimum_required(VERSION 3.13)
project(skifree_wasm)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten specific settings
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=['png'] -s USE_WEBGL2=1 -s FULL_ES3=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file assets")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/obstacle.cpp
    src/hud.cpp
    src/spritesheet.cpp
)

# Include directories
include_directories(include)

# Create executable
add_executable(skifree ${SOURCES})

target_link_libraries(skifree SDL2 SDL2_ttf) 