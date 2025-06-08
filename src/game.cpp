#include "game.hpp"
#include "player.hpp"
#include <SDL.h>
#include <emscripten.h>

Game::Game() : running(true), window(nullptr), renderer(nullptr), player(nullptr) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SkiFree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    player = new Player();
}

Game::~Game() {
    delete player;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::main_loop(void* arg) {
    Game* game = static_cast<Game*>(arg);
    game->processEvents();
    game->update();
    game->render();
    if (!game->running) {
        emscripten_cancel_main_loop();
    }
}

void Game::run() {
    emscripten_set_main_loop_arg(Game::main_loop, this, 0, 1);
}

void Game::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    player->update(-1.0f); // Move left
                    break;
                case SDLK_RIGHT:
                    player->update(1.0f);  // Move right
                    break;
                case SDLK_UP:
                    player->update(0.0f);  // Speed up
                    break;
                case SDLK_DOWN:
                    player->update(0.0f);  // Slow down
                    break;
            }
        }
    }
}

void Game::update() {
    // TODO: Implement game update logic
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    player->render(renderer);
    SDL_RenderPresent(renderer);
}

// Add Player as a member
Player* player; 