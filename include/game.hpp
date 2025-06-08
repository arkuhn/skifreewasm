#pragma once
#include <SDL.h>
class Player;
class HUD;

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

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Player* player;
    HUD* hud;
    
    // Game state
    int score;
    int speed;
    int distance;
}; 