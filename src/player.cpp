#include "player.hpp"

Player::Player(int screen_width, int screen_height) {
    reset(screen_width);
    rect.y = screen_height - 150; // Keep player vertically positioned near the bottom
}

void Player::reset(int screen_width) {
    rect.x = screen_width / 2;
    rect.w = 20;
    rect.h = 20;
    speed = 2.0f;
    state = PlayerState::SKIING;
}

void Player::update(float steer, int screen_width) {
    if (state == PlayerState::SKIING) {
        rect.x += static_cast<int>(steer * (speed / 2.0f));
        if (rect.x < 0) rect.x = 0;
        if (rect.x + rect.w > screen_width) rect.x = screen_width - rect.w;
    }
}

void Player::crash() {
    state = PlayerState::CRASHED;
    speed = 0;
}

void Player::setSpeed(int s) {
    if (state == PlayerState::SKIING) {
        speed = static_cast<float>(s);
    }
}

int Player::getSpeed() const {
    return static_cast<int>(speed);
}

PlayerState Player::getState() const {
    return state;
}

SDL_Rect Player::getRect() const {
    return rect;
}

void Player::render(SDL_Renderer* renderer) {
    if (state == PlayerState::CRASHED) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red when crashed
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue when skiing
    }
    SDL_RenderFillRect(renderer, &rect);
} 