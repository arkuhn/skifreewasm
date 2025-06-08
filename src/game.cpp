#include "game.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "obstacle.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <algorithm>
#include <cstdlib>

Game::Game() : running(true), window(nullptr), renderer(nullptr), player(nullptr), hud(nullptr),
    score(0), speed(2), world_y(0), steer(0.0f) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    window = SDL_CreateWindow("SkiFree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    player = new Player(SCREEN_WIDTH, SCREEN_HEIGHT);
    hud = new HUD(renderer);
    srand(time(nullptr));
}

Game::~Game() {
    for (auto obs : obstacles) delete obs;
    delete player;
    delete hud;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
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
    emscripten_set_main_loop_arg(Game::main_loop, this, -1, 1);
}

void Game::processEvents() {
    SDL_Event event;
    steer = 0.0f;

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_LEFT]) {
        steer = -1.0f;
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        steer = 1.0f;
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (player->getState() == PlayerState::CRASHED && event.key.keysym.sym == SDLK_SPACE) {
                player->reset(SCREEN_WIDTH);
                speed = 2;
            } else {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        speed = std::min(speed + 1, 20);
                        break;
                    case SDLK_DOWN:
                        speed = std::max(speed - 1, 1);
                        break;
                }
            }
        }
    }
}

void Game::update() {
    if (player->getState() == PlayerState::SKIING) {
        world_y += speed;
        score += speed;
    }
    
    player->setSpeed(speed);
    player->update(steer, SCREEN_WIDTH);
    
    manageObstacles();
    checkCollisions();
}

void Game::manageObstacles() {
    // Generate new obstacles
    while (obstacles.empty() || obstacles.back()->getY() < world_y + SCREEN_HEIGHT) {
        int x = rand() % SCREEN_WIDTH;
        int y = (obstacles.empty() ? world_y : obstacles.back()->getY()) + 100 + (rand() % 200);
        obstacles.push_back(new Obstacle(x, y));
    }

    // Remove old obstacles
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [this](Obstacle* obs) {
            if (obs->getY() < this->world_y - 100) {
                delete obs;
                return true;
            }
            return false;
        }), obstacles.end());
}

void Game::checkCollisions() {
    if (player->getState() == PlayerState::SKIING) {
        SDL_Rect player_rect = player->getRect();
        for (auto obs : obstacles) {
            SDL_Rect obs_rect = obs->getRect();
            obs_rect.y -= world_y; // Adjust to screen coordinates
            if (SDL_HasIntersection(&player_rect, &obs_rect)) {
                player->crash();
                speed = 0;
                break;
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    
    for (auto obs : obstacles) {
        obs->render(renderer, world_y);
    }
    
    player->render(renderer);
    hud->render(score, speed, world_y);
    SDL_RenderPresent(renderer);
}

// Add Player as a member
Player* player;
// Add steer as a member variable
float steer = 0.0f; 