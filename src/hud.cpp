#include "hud.hpp"
#include <SDL_ttf.h>
#include <sstream>

static TTF_Font* g_font = nullptr;

HUD::HUD(SDL_Renderer* renderer) : renderer(renderer) {
    TTF_Init();
    setFont("assets/FreeSans.ttf", 18);
}

void HUD::setFont(const char* fontPath, int size) {
    if (g_font) {
        TTF_CloseFont(g_font);
    }
    g_font = TTF_OpenFont(fontPath, size);
}

SDL_Texture* HUD::renderText(const char* text, SDL_Color color) {
    if (!g_font) return nullptr;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(g_font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

void HUD::renderStat(const char* label, int value, int y) {
    std::stringstream ss;
    ss << label << ": " << value;
    SDL_Color color = {0, 0, 0, 255}; // Black text
    SDL_Texture* texture = renderText(ss.str().c_str(), color);
    if (texture) {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect dest = {10, y, w, h};  // Top left
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
        SDL_DestroyTexture(texture);
    }
}

void HUD::render(int score, int speed, int distance) {
    renderStat("Score", score, 10);
    renderStat("Speed", speed, 40);
    renderStat("Dist", distance, 70);
} 