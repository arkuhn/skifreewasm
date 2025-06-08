#include "game.hpp"
#include "player.hpp"
#include "hud.hpp"
#include "obstacle.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <emscripten.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <SDL_image.h>
#include <iostream>

Game::Game() : running(false), window(nullptr), renderer(nullptr), player(nullptr), hud(nullptr), sprites(nullptr), yeti(nullptr),
    score(0), speed(2), world_y(0), steer(0.0f), jump_pressed(false), slalom_bonus(0), yeti_spawn_distance(2000),
    game_state(GameState::PLAYING) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return;
    }

    // Create window
    window = SDL_CreateWindow("SkiFree WebAssembly", 
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Initialize sprite sheet
    sprites = new SpriteSheet();
    if (!sprites->load(renderer, "assets/kenney_tiny-ski/Tilemap/tilemap.png")) {
        std::cerr << "Failed to load sprites, using fallback rendering" << std::endl;
    }

    // Initialize game objects
    player = new Player(SCREEN_WIDTH, SCREEN_HEIGHT);
    hud = new HUD(renderer);

    // Initialize obstacles
    srand(time(nullptr));
    for (int i = 0; i < 20; ++i) {
        int x = rand() % (SCREEN_WIDTH - 32);
        int y = (rand() % 2000) + SCREEN_HEIGHT; // Spread obstacles ahead
        ObstacleType obstacleType = static_cast<ObstacleType>(rand() % 4); // 0-3 for TREE_SMALL, TREE_LARGE, ROCK, STUMP
        obstacles.push_back(new Obstacle(x, y, obstacleType));
    }

    running = true;
}

Game::~Game() {
    for (auto obs : obstacles) delete obs;
    delete yeti;
    delete player;
    delete hud;
    delete sprites;
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
    
    // Continuous steering
    if (keystates[SDL_SCANCODE_LEFT]) steer = -1.0f;
    if (keystates[SDL_SCANCODE_RIGHT]) steer = 1.0f;
    
    // Speed control
    if (keystates[SDL_SCANCODE_UP]) speed = std::min(speed + 1, 25);
    if (keystates[SDL_SCANCODE_DOWN]) speed = std::max(speed - 1, 1);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    if (player->getState() == PlayerState::CRASHED) {
                        player->reset(SCREEN_WIDTH);
                        game_state = GameState::PLAYING;
                        speed = 2;
                        score = 0;
                        world_y = 0;
                        // Clear obstacles
                        for (auto obs : obstacles) delete obs;
                        obstacles.clear();
                        delete yeti;
                        yeti = nullptr;
                        yeti_spawn_distance = 2000;
                    } else {
                        player->jump();
                    }
                    break;
            }
        }
    }
}

void Game::update() {
    if (player->getState() == PlayerState::SKIING || player->getState() == PlayerState::JUMPING) {
        world_y += speed;
        score += speed;
        
        // Check for yeti spawn
        if (world_y > yeti_spawn_distance && !yeti && speed > 10) {
            spawnYeti();
            game_state = GameState::YETI_CHASE;
        }
    }
    
    player->setSpeed(speed);
    player->update(steer, SCREEN_WIDTH, world_y);
    
    if (game_state == GameState::YETI_CHASE) {
        updateYetiChase();
    }
    
    manageObstacles();
    checkCollisions();
}

void Game::spawnYeti() {
    int yeti_x = SCREEN_WIDTH / 2;
    int yeti_y = world_y + SCREEN_HEIGHT + 100; // Spawn behind player
    yeti = new Obstacle(yeti_x, yeti_y, ObstacleType::YETI);
}

void Game::updateYetiChase() {
    if (yeti) {
        yeti->update(world_y, speed);
        
        // If yeti catches up, game over
        if (yeti->getY() - world_y < SCREEN_HEIGHT - 100) {
            player->crash();
            game_state = GameState::GAME_OVER;
        }
    }
}

ObstacleType Game::getRandomObstacleType() {
    int rand_val = rand() % 100;
    
    if (rand_val < 30) return ObstacleType::TREE_SMALL;
    if (rand_val < 45) return ObstacleType::TREE_LARGE;
    if (rand_val < 55) return ObstacleType::ROCK;
    if (rand_val < 60) return ObstacleType::STUMP;
    if (rand_val < 70) return ObstacleType::MOGUL;
    if (rand_val < 80) return ObstacleType::SLALOM_FLAG;
    if (rand_val < 95) return ObstacleType::OTHER_SKIER;
    
    return ObstacleType::TREE_SMALL; // Default
}

void Game::manageObstacles() {
    // Generate new obstacles
    while (obstacles.empty() || obstacles.back()->getY() < world_y + SCREEN_HEIGHT * 2) {
        int x = rand() % (SCREEN_WIDTH - 50);
        int y = (obstacles.empty() ? world_y + SCREEN_HEIGHT : obstacles.back()->getY()) + 80 + (rand() % 120);
        
        ObstacleType type = getRandomObstacleType();
        obstacles.push_back(new Obstacle(x, y, type));
    }

    // Update obstacles
    for (auto obs : obstacles) {
        obs->update(world_y, speed);
    }

    // Remove old obstacles
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [this](Obstacle* obs) {
            if (obs->getY() < this->world_y - 200) {
                delete obs;
                return true;
            }
            return false;
        }), obstacles.end());
}

void Game::checkCollisions() {
    if (player->getState() != PlayerState::SKIING && player->getState() != PlayerState::JUMPING) return;
    
    SDL_Rect player_rect = player->getRect();
    
    for (auto obs : obstacles) {
        SDL_Rect obs_rect = obs->getRect();
        obs_rect.y -= world_y; // Adjust to screen coordinates
        
        if (SDL_HasIntersection(&player_rect, &obs_rect)) {
            if (obs->getType() == ObstacleType::SLALOM_FLAG) {
                score += obs->getPointValue();
                slalom_bonus += 100;
            } else if (obs->getType() == ObstacleType::MOGUL) {
                if (player->getState() == PlayerState::JUMPING) {
                    score += obs->getPointValue() * 2; // Double points for jumping moguls
                }
            } else if (obs->shouldCollide()) {
                if (player->getState() == PlayerState::JUMPING) {
                    score += 50; // Bonus for jumping over obstacles
                } else {
                    player->crash();
                    break;
                }
            }
        }
    }
    
    // Check yeti collision
    if (yeti) {
        SDL_Rect yeti_rect = yeti->getRect();
        yeti_rect.y -= world_y;
        if (SDL_HasIntersection(&player_rect, &yeti_rect)) {
            player->crash();
            game_state = GameState::GAME_OVER;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White background (snow)
    SDL_RenderClear(renderer);
    
    Terrain::renderSnowPattern(renderer, world_y, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // Render obstacles with sprites
    for (auto& obstacle : obstacles) {
        obstacle->render(renderer, world_y, sprites);
    }
    
    // Render player with sprites
    player->render(renderer, world_y, sprites);
    
    hud->render(score + slalom_bonus, static_cast<int>(speed), world_y);
    
    SDL_RenderPresent(renderer);
} 