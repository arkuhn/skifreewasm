#pragma once
#include <SDL.h>

class Player {
public:
    Player();
    void update(float direction);
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
    float speed;
}; 