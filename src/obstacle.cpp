#include "obstacle.hpp"
#include <cstdlib>

Obstacle::Obstacle(int x, int y) {
    rect = {x, y, 0, 0};
    if (rand() % 2 == 0) {
        type = ObstacleType::TREE;
        rect.w = 20;
        rect.h = 50;
    } else {
        type = ObstacleType::ROCK;
        rect.w = 30;
        rect.h = 20;
    }
}


void Obstacle::render(SDL_Renderer* renderer, int world_y) {
    SDL_Rect on_screen_rect = rect;
    on_screen_rect.y -= world_y;

    if (type == ObstacleType::TREE) {
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown for tree
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray for rock
    }
    SDL_RenderFillRect(renderer, &on_screen_rect);
}

SDL_Rect Obstacle::getRect() const {
    return rect;
}

int Obstacle::getY() const {
    return rect.y;
} 