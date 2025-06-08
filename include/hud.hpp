#pragma once
#include <SDL.h>
#include <string>

class HUD {
public:
    HUD(SDL_Renderer* renderer);
    void render(int score, int speed, int distance);
    void setFont(const char* fontPath, int size);

private:
    SDL_Renderer* renderer;
    SDL_Texture* renderText(const char* text, SDL_Color color);
    void renderStat(const char* label, int value, int y);
    
    int score;
    int speed;
    int distance;
}; 