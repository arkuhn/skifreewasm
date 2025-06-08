#pragma once
#include <SDL.h>

enum class ObstacleType {
    TREE,
    ROCK
};

class Obstacle {
public:
    Obstacle(int x, int y);
    void render(SDL_Renderer* renderer, int world_y);
    SDL_Rect getRect() const;
    int getY() const;

private:
    SDL_Rect rect;
    ObstacleType type;
}; 