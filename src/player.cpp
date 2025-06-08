#include "player.hpp"

Player::Player() : speed(0.0f) {
    rect = {400, 300, 20, 20}; // Start in middle of screen, 20x20 size
}

void Player::update(float direction) {
    rect.x += direction * 5; // Move left/right by 5 pixels
    // TODO: Implement speed changes for up/down
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    SDL_RenderFillRect(renderer, &rect);
} 