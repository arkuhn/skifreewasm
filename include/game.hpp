#pragma once
#include <SDL.h>
class Player;

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
}; 