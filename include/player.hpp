#pragma once
#include <SDL.h>
#include <vector>

class SpriteSheet;

enum class PlayerState {
    SKIING,
    JUMPING,
    CRASHED,
    TUMBLING
};

enum class PlayerDirection {
    LEFT,
    RIGHT,
    DOWN
};

struct SkiTrack {
    int x, y;
    int age;
};

class Player {
public:
    Player(int screen_width, int screen_height);
    void update(float steer, int screen_width, int world_y);
    void render(SDL_Renderer* renderer, int world_y, SpriteSheet* sprites = nullptr);
    void setSpeed(int s);
    void jump();
    void crash();
    void tumble();
    void reset(int screen_width);

    PlayerState getState() const { return state; }
    SDL_Rect getRect() const;
    int getSpeed() const { return speed; }
    float getMomentumX() const { return momentum_x; }
    float getMomentumY() const { return momentum_y; }

private:
    void updateSkiTracks(int world_y);
    void renderSkiTracks(SDL_Renderer* renderer, int world_y);

    SDL_Rect rect;
    float speed;
    float momentum_x, momentum_y;
    PlayerState state;
    PlayerDirection direction;
    int jump_height;
    int crash_timer;
    std::vector<SkiTrack> ski_tracks;
}; 