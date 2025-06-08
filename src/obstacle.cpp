#include "obstacle.hpp"
#include "spritesheet.hpp"
#include <cstdlib>
#include <cmath>

Obstacle::Obstacle(int x, int y, ObstacleType obs_type) : type(obs_type), movement_x(0), hit(false) {
    rect.x = x;
    rect.y = y;
    rect.w = 32; // Scale up 16x16 tiles to 32x32 for visibility
    rect.h = 32;
    
    switch (type) {
        case ObstacleType::TREE_SMALL:
            rect.w = 12; rect.h = 30;
            break;
        case ObstacleType::TREE_LARGE:
            rect.w = 25; rect.h = 60;
            break;
        case ObstacleType::ROCK:
            rect.w = 20; rect.h = 15;
            break;
        case ObstacleType::STUMP:
            rect.w = 15; rect.h = 10;
            break;
        case ObstacleType::MOGUL:
            rect.w = 40; rect.h = 20;
            break;
        case ObstacleType::SLALOM_FLAG:
            rect.w = 8; rect.h = 50;
            break;
        case ObstacleType::OTHER_SKIER:
            rect.w = 12; rect.h = 12;
            movement_x = (rand() % 3 - 1) * 2; // Random movement
            break;
        case ObstacleType::YETI:
            rect.w = 40; rect.h = 60;
            movement_x = 3; // Aggressive movement toward player
            break;
    }
}

void Obstacle::update(int world_y, int player_speed) {
    switch (type) {
        case ObstacleType::OTHER_SKIER:
            rect.x += movement_x;
            // Occasionally change direction
            if (rand() % 60 == 0) {
                movement_x = (rand() % 3 - 1) * 2;
            }
            break;
        case ObstacleType::YETI:
            // Yeti moves faster than normal world scroll, catching up to player
            rect.y -= (player_speed + 2);
            // Also move side to side to chase player
            if (rand() % 30 == 0) {
                movement_x = (rand() % 7 - 3);
            }
            rect.x += movement_x;
            break;
        default:
            break;
    }
}

bool Obstacle::shouldCollide() const {
    return type != ObstacleType::SLALOM_FLAG && type != ObstacleType::MOGUL;
}

int Obstacle::getPointValue() const {
    switch (type) {
        case ObstacleType::SLALOM_FLAG: return hit ? 0 : 100;
        case ObstacleType::MOGUL: return 10; // Bonus for going over moguls
        default: return 0;
    }
}

void Obstacle::render(SDL_Renderer* renderer, int world_y) {
    SDL_Rect on_screen_rect = rect;
    on_screen_rect.y -= world_y;
    
    // Don't render if off screen
    if (on_screen_rect.y < -100 || on_screen_rect.y > 700) return;
    
    // Fallback colored rectangles (will be replaced by sprites in game.cpp)
    switch (type) {
        case ObstacleType::TREE_SMALL:
            SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
            break;
        case ObstacleType::TREE_LARGE:
            SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
            break;
        case ObstacleType::ROCK:
            SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255);
            break;
        case ObstacleType::STUMP:
            SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255);
            break;
        case ObstacleType::MOGUL:
            SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);
            break;
        case ObstacleType::SLALOM_FLAG:
            if (!hit) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            break;
        case ObstacleType::OTHER_SKIER:
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
            break;
        case ObstacleType::YETI:
            SDL_SetRenderDrawColor(renderer, 248, 248, 255, 255);
            break;
    }
    SDL_RenderFillRect(renderer, &on_screen_rect);
}

void Obstacle::render(SDL_Renderer* renderer, int world_y, SpriteSheet* sprites) {
    SDL_Rect render_rect = {rect.x, rect.y - world_y, 32, 32};
    
    // Only render if on screen
    if (render_rect.y > -32 && render_rect.y < 600 + 32) {
        if (sprites) {
            SpriteID spriteId = SpriteID::TREE_SMALL;
            
            switch (type) {
                case ObstacleType::TREE_SMALL:
                    spriteId = SpriteID::TREE_SMALL;
                    break;
                case ObstacleType::TREE_LARGE:
                    spriteId = SpriteID::TREE_LARGE;
                    break;
                case ObstacleType::ROCK:
                    spriteId = SpriteID::ROCK;
                    break;
                case ObstacleType::STUMP:
                    spriteId = SpriteID::TREE_SMALL; // Use tree_small for stump
                    break;
                case ObstacleType::SLALOM_FLAG:
                    if (!hit) {
                        spriteId = SpriteID::FLAG;
                    } else {
                        return; // Don't render hit flags
                    }
                    break;
                case ObstacleType::YETI:
                    spriteId = SpriteID::YETI;
                    break;
                case ObstacleType::OTHER_SKIER:
                    spriteId = SpriteID::OTHER_SKIER;
                    break;
                case ObstacleType::MOGUL:
                    return; // No sprite for mogul, just a physics object
                default:
                    return; // Don't render unknown obstacles
            }
            
            sprites->render(spriteId, render_rect.x, render_rect.y, render_rect.w, render_rect.h);
        } else {
            // Fallback to colored rectangles
            switch (type) {
                case ObstacleType::TREE_SMALL:
                    SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Forest green
                    break;
                case ObstacleType::TREE_LARGE:
                    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Dark green
                    break;
                case ObstacleType::ROCK:
                    SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255); // Gray
                    break;
                case ObstacleType::STUMP:
                    SDL_SetRenderDrawColor(renderer, 160, 82, 45, 255); // Brown
                    break;
                case ObstacleType::MOGUL:
                    SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255); // Light blue
                    break;
                case ObstacleType::SLALOM_FLAG:
                    if (!hit) {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                    } else {
                        return; // Don't render hit flags
                    }
                    break;
                case ObstacleType::OTHER_SKIER:
                    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange
                    break;
                case ObstacleType::YETI:
                    SDL_SetRenderDrawColor(renderer, 248, 248, 255, 255); // White
                    break;
            }
            SDL_RenderFillRect(renderer, &render_rect);
        }
    }
}

SDL_Rect Obstacle::getRect() const {
    return rect;
}

int Obstacle::getY() const {
    return rect.y;
}

// Terrain implementation
int Terrain::getGroundHeight(int x, int y) {
    // Simple sine wave for terrain variation
    return static_cast<int>(sin(x * 0.01f + y * 0.005f) * 10);
}

void Terrain::renderSnowPattern(SDL_Renderer* renderer, int world_y, int screen_width, int screen_height) {
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255); // Snow white
    
    // Render snow particles
    for (int i = 0; i < 50; i++) {
        int x = (world_y * 2 + i * 47) % screen_width;
        int y = (world_y + i * 23) % screen_height;
        SDL_Rect snowflake = {x, y, 2, 2};
        SDL_RenderFillRect(renderer, &snowflake);
    }
} 