#pragma once
#include <SDL.h>

class SpriteSheet;

enum class ObstacleType {
    TREE_SMALL,
    TREE_LARGE,
    ROCK,
    STUMP,
    MOGUL,
    SLALOM_FLAG,
    OTHER_SKIER,
    YETI
};

class Obstacle {
public:
    Obstacle(int x, int y, ObstacleType type = ObstacleType::TREE_SMALL);
    void update(int world_y, int player_speed);
    void render(SDL_Renderer* renderer, int world_y);
    void render(SDL_Renderer* renderer, int world_y, SpriteSheet* sprites);
    SDL_Rect getRect() const;
    int getY() const;
    ObstacleType getType() const { return type; }
    bool shouldCollide() const;
    int getPointValue() const;

private:
    SDL_Rect rect;
    ObstacleType type;
    int movement_x; // For moving obstacles like other skiers and yeti
    bool hit; // For slalom flags
};

// Terrain generation helper
class Terrain {
public:
    static int getGroundHeight(int x, int y);
    static void renderSnowPattern(SDL_Renderer* renderer, int world_y, int screen_width, int screen_height);
}; 