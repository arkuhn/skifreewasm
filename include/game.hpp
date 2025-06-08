#pragma once
#include <SDL.h>
#include <vector>
#include "obstacle.hpp"
#include "spritesheet.hpp"

class Player;
class Obstacle;
class HUD;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int YETI_CHASE_SPEED = 15;

enum class GameState {
    PLAYING,
    YETI_CHASE,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();
    void run();
    static void main_loop(void* arg);

private:
    void processEvents();
    void update();
    void render();
    void manageObstacles();
    void checkCollisions();
    void spawnYeti();
    void updateYetiChase();
    ObstacleType getRandomObstacleType();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    
    Player* player;
    HUD* hud;
    SpriteSheet* sprites;
    std::vector<Obstacle*> obstacles;
    Obstacle* yeti;

    float steer;
    bool jump_pressed;
    
    // Game state
    GameState game_state;
    int score;
    int speed;
    int world_y; // World scroll position
    int slalom_bonus;
    int yeti_spawn_distance;
    
    // Input handling
    bool keys_held[SDL_NUM_SCANCODES];
}; 