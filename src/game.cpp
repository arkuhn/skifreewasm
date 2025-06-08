#include "game.hpp"
#include "player.hpp"
#include "hud.hpp"
#include <SDL.h>
#include <emscripten.h>

Game::Game() : running(true), window(nullptr), renderer(nullptr), player(nullptr), hud(nullptr),
    score(0), speed(0), distance(0) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SkiFree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    player = new Player();
    hud = new HUD(renderer);
}

Game::~Game() {
    delete player;
    delete hud;
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
                    player->update(-1.0f);
                    break;
                case SDLK_RIGHT:
                    player->update(1.0f);
                    break;
                case SDLK_UP:
                    speed = std::min(speed + 1, 100);
                    break;
                case SDLK_DOWN:
                    speed = std::max(speed - 1, 0);
                    break;
            }
        }
    }
}

void Game::update() {
    // Update game state
    distance += speed;
    if (speed > 0) {
        score += speed;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    player->render(renderer);
    hud->render(score, speed, distance);
    SDL_RenderPresent(renderer);
}

// Add Player as a member
Player* player; 