#include "obstacle.hpp"

Obstacle::Obstacle(int x, int y, int w, int h) {
    rect = {x, y, w, h};
}

void Obstacle::update(float deltaTime) {
    // TODO: Implement obstacle logic
}

void Obstacle::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    SDL_RenderFillRect(renderer, &rect);
} 