#include "spritesheet.hpp"
#include <SDL_image.h>
#include <iostream>

SpriteSheet::SpriteSheet() : texture(nullptr), renderer(nullptr) {
}

SpriteSheet::~SpriteSheet() {
    cleanup();
}

bool SpriteSheet::load(SDL_Renderer* renderer, const std::string& path) {
    this->renderer = renderer;
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return false;
    }
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

SpriteCoord SpriteSheet::getSpriteCoord(SpriteID id) {
    // Calculate tile position based on tile index
    // Tile layout: 12 tiles horizontally, 11 tiles vertically
    // Each tile is 16x16 with 1px spacing
    
    int tileIndex = 0;
    switch (id) {
        case SpriteID::SNOW:
            tileIndex = 0;
            break;
        case SpriteID::TREE_SMALL:
            tileIndex = 8;
            break;
        case SpriteID::TREE_LARGE:
            tileIndex = 9;
            break;
        case SpriteID::ROCK:
            tileIndex = 10;
            break;
        case SpriteID::FLAG:
            tileIndex = 14;
            break;
        case SpriteID::YETI:
            tileIndex = 60;
            break;
        case SpriteID::OTHER_SKIER:
            tileIndex = 70;
            break;
        case SpriteID::SKIER_DOWN:
            tileIndex = 82;
            break;
        case SpriteID::SKIER_LEFT:
            tileIndex = 82;
            break;
        case SpriteID::SKIER_RIGHT:
            tileIndex = 82;
            break;
        case SpriteID::SKIER_JUMP:
            tileIndex = 82;
            break;
        case SpriteID::SKIER_CRASH:
            tileIndex = 68;
            break;
        default:
            tileIndex = 0;
            break;
    }
    
    // Calculate grid position
    int col = tileIndex % TILES_HORIZONTAL;
    int row = tileIndex / TILES_HORIZONTAL;
    
    // Calculate pixel coordinates including spacing
    int x = col * (TILE_SIZE + TILE_SPACING);
    int y = row * (TILE_SIZE + TILE_SPACING);
    
    return {x, y, TILE_SIZE, TILE_SIZE};
}

void SpriteSheet::render(SpriteID id, int x, int y, int width, int height) {
    if (!texture || !renderer) {
        // Fallback to colored rectangles if texture loading failed
        SDL_Rect rect = {x, y, width, height};
        
        switch (id) {
            case SpriteID::SKIER_DOWN:
            case SpriteID::SKIER_LEFT:
            case SpriteID::SKIER_RIGHT:
            case SpriteID::SKIER_JUMP:
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue for skier
                break;
            case SpriteID::SKIER_CRASH:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for crash
                break;
            case SpriteID::TREE_SMALL:
            case SpriteID::TREE_LARGE:
                SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Green for trees
                break;
            case SpriteID::ROCK:
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray for rock
                break;
            case SpriteID::FLAG:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for flag
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for snow
                break;
        }
        
        SDL_RenderFillRect(renderer, &rect);
        return;
    }
    
    SpriteCoord coord = getSpriteCoord(id);
    SDL_Rect srcRect = {coord.x, coord.y, coord.w, coord.h};
    SDL_Rect destRect = {x, y, width, height};
    
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void SpriteSheet::renderDebugGrid(int x, int y, int cellSize) {
    if (!texture || !renderer) return;
    int idx = 0;
    for (int row = 0; row < TILES_VERTICAL; ++row) {
        for (int col = 0; col < TILES_HORIZONTAL; ++col) {
            int px = x + col * cellSize;
            int py = y + row * cellSize;
            int tileIndex = row * TILES_HORIZONTAL + col;
            SDL_Rect srcRect = {col * (TILE_SIZE + TILE_SPACING), row * (TILE_SIZE + TILE_SPACING), TILE_SIZE, TILE_SIZE};
            SDL_Rect destRect = {px, py, cellSize, cellSize};
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        }
    }
}

void SpriteSheet::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
} 