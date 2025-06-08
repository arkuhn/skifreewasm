#include "player.hpp"
#include "spritesheet.hpp"
#include <algorithm>
#include <cmath>

Player::Player(int screen_width, int screen_height) : momentum_x(0), momentum_y(0), jump_height(0), crash_timer(0), direction(PlayerDirection::DOWN) {
    reset(screen_width);
    rect.y = screen_height - 150;
} 

void Player::reset(int screen_width) {
    rect.x = screen_width / 2;
    rect.w = 32; // Scale up 16x16 tiles to 32x32 for visibility
    rect.h = 32;
    speed = 2.0f;
    momentum_x = 0;
    momentum_y = 0;
    jump_height = 0;
    crash_timer = 0;
    direction = PlayerDirection::DOWN;
    state = PlayerState::SKIING;
    ski_tracks.clear();
}

void Player::update(float steer, int screen_width, int world_y) {
    // Update direction based on steering
    if (steer < -0.1f) direction = PlayerDirection::LEFT;
    else if (steer > 0.1f) direction = PlayerDirection::RIGHT;
    else direction = PlayerDirection::DOWN;
    
    switch (state) {
        case PlayerState::SKIING:
            // Apply steering with momentum
            momentum_x += steer * 0.3f;
            momentum_x *= 0.85f; // Friction
            
            rect.x += static_cast<int>(momentum_x * speed * 0.5f);
            if (rect.x < 0) {
                rect.x = 0;
                momentum_x = 0;
            }
            if (rect.x + rect.w > screen_width) {
                rect.x = screen_width - rect.w;
                momentum_x = 0;
            }
            
            updateSkiTracks(world_y);
            break;
            
        case PlayerState::JUMPING:
            jump_height -= 2;
            if (jump_height <= 0) {
                jump_height = 0;
                state = PlayerState::SKIING;
            }
            // Apply momentum while jumping
            rect.x += static_cast<int>(momentum_x * 0.7f);
            if (rect.x < 0) rect.x = 0;
            if (rect.x + rect.w > screen_width) rect.x = screen_width - rect.w;
            break;
            
        case PlayerState::CRASHED:
        case PlayerState::TUMBLING:
            crash_timer--;
            if (crash_timer <= 0) {
                state = PlayerState::SKIING;
                speed = 1;
            }
            break;
    }
}

void Player::jump() {
    if (state == PlayerState::SKIING) {
        state = PlayerState::JUMPING;
        jump_height = 30;
    }
}

void Player::crash() {
    if (state != PlayerState::CRASHED) {
        state = PlayerState::CRASHED;
        crash_timer = 60; // 1 second at 60fps
        momentum_x *= 0.1f;
    }
}

void Player::tumble() {
    state = PlayerState::TUMBLING;
    crash_timer = 90;
}

void Player::setSpeed(int s) {
    if (state == PlayerState::SKIING || state == PlayerState::JUMPING) {
        speed = static_cast<float>(s);
    }
}

SDL_Rect Player::getRect() const {
    SDL_Rect adjusted_rect = rect;
    adjusted_rect.y -= jump_height;
    return adjusted_rect;
}

void Player::updateSkiTracks(int world_y) {
    // Add new ski track
    if (state == PlayerState::SKIING) {
        ski_tracks.push_back({rect.x + rect.w/2 - 2, world_y + rect.y, 0});
        ski_tracks.push_back({rect.x + rect.w/2 + 2, world_y + rect.y, 0});
    }
    
    // Age and remove old tracks
    for (auto& track : ski_tracks) {
        track.age++;
    }
    ski_tracks.erase(std::remove_if(ski_tracks.begin(), ski_tracks.end(),
        [](const SkiTrack& track) { return track.age > 300; }), ski_tracks.end());
}

void Player::renderSkiTracks(SDL_Renderer* renderer, int world_y) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255); // Light blue tracks
    for (const auto& track : ski_tracks) {
        int screen_y = track.y - world_y;
        if (screen_y >= -10 && screen_y <= 610) {
            SDL_Rect track_rect = {track.x, screen_y, 2, 4};
            SDL_RenderFillRect(renderer, &track_rect);
        }
    }
}

void Player::render(SDL_Renderer* renderer, int world_y, SpriteSheet* sprites) {
    renderSkiTracks(renderer, world_y);
    
    if (sprites) {
        SpriteID spriteId = SpriteID::SKIER_DOWN;
                
        if (state == PlayerState::CRASHED) {
            spriteId = SpriteID::SKIER_CRASH;
        } else if (state == PlayerState::JUMPING) {
            spriteId = SpriteID::SKIER_JUMP;
        } else { // PlayerState::SKIING
            switch (direction) {
                case PlayerDirection::LEFT:
                    spriteId = SpriteID::SKIER_LEFT;
                    break;
                case PlayerDirection::RIGHT:
                    spriteId = SpriteID::SKIER_RIGHT;
                    break;
                case PlayerDirection::DOWN:
                default:
                    spriteId = SpriteID::SKIER_DOWN;
                    break;
            }
        }
        
        SDL_Rect rect = getRect();
        sprites->render(spriteId, rect.x, rect.y, rect.w, rect.h);
    } else {
        // Fallback to colored rectangles
        SDL_Rect rect = getRect();
        if (state == PlayerState::CRASHED) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
        }
        SDL_RenderFillRect(renderer, &rect);
    }
} 