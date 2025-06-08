#pragma once
#include <SDL.h>
#include <vector>

class Player;
class Obstacle;
class HUD;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

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

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    
    Player* player;
    HUD* hud;
    std::vector<Obstacle*> obstacles;

    float steer;
    
    // Game state
    int score;
    int speed;
    int world_y; // World scroll position
}; 