#pragma once
#include <SDL.h>

enum class PlayerState {
    SKIING,
    CRASHED
};

class Player {
public:
    Player(int screen_width, int screen_height);
    void update(float steer, int screen_width);
    void render(SDL_Renderer* renderer);
    void setSpeed(int s);
    int getSpeed() const;
    void crash();
    void reset(int screen_width);

    PlayerState getState() const;
    SDL_Rect getRect() const;

private:
    SDL_Rect rect;
    float speed;
    PlayerState state;
}; 