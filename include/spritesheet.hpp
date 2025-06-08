#pragma once
#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <SDL2/SDL_image.h>
#else
#include <SDL_image.h>
#endif
#include <string>

enum class SpriteID {
    // Skier sprites (row 3, approximately tiles 24-35)
    SKIER_DOWN,   // tile 25
    SKIER_LEFT,   // tile 26  
    SKIER_RIGHT,  // tile 27
    SKIER_JUMP,   // tile 28
    SKIER_CRASH,  // tile 29
    
    // Trees and obstacles (row 2, approximately tiles 12-23)
    TREE_SMALL,   // tile 13
    TREE_LARGE,   // tile 14
    ROCK,         // tile 15
    FLAG,         // tile 16
    
    // Added for new obstacle types
    YETI,
    OTHER_SKIER,
    
    // Terrain (row 1, approximately tiles 0-11)
    SNOW          // tile 1
};

struct SpriteCoord {
    int x, y, w, h;
};

class SpriteSheet {
private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    
    // Tile size and spacing from Tilesheet.txt
    static const int TILE_SIZE = 16;
    static const int TILE_SPACING = 1;
    static const int TILES_HORIZONTAL = 12;
    static const int TILES_VERTICAL = 11;
    
    SpriteCoord getSpriteCoord(SpriteID id);
    
public:
    SpriteSheet();
    ~SpriteSheet();
    
    bool load(SDL_Renderer* renderer, const std::string& path);
    void render(SpriteID id, int x, int y, int width = 32, int height = 32);
    void renderDebugGrid(int x, int y, int cellSize = 32);
    void cleanup();
}; 