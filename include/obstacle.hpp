#pragma once
#include <SDL.h>

class Obstacle {
public:
    Obstacle(int x, int y, int w, int h);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
private:
    SDL_Rect rect;
}; 