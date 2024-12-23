#include "text.h"
#include "../engine/src/constants.h"
#include <iostream>
#include <sstream>

Text::Text(const std::string& fontPath, const std::string messageText, int fontSize, Vector2 position, SDL_Color color)
    : font(nullptr), message(messageText), position(position), color(color) { 
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        throw std::runtime_error("Failed to load font: " + std::string(TTF_GetError()));
    }
    leftToRight = 1;
}

Text::~Text() {
    TTF_CloseFont(font);
}

void Text::render(SDL_Renderer* renderer) {
    std::istringstream stream(message);
    std::string line;
    int offsetY = (int)position.getY();  

    while (std::getline(stream, line)) {
        // SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
        SDL_Surface* surface = TTF_RenderText_Solid(font, line.c_str(), color);
        if (!surface) {
            throw std::runtime_error("Failed to create surface: " + std::string(TTF_GetError()));
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            throw std::runtime_error("Failed to create tedture: " + std::string(TTF_GetError()));
        }

        // SDL_Rect destRect = {(int)position.getX(), (int)position.getY(), surface->w, surface->h};
        SDL_Rect destRect = { (int)position.getX(), offsetY, surface->w, surface->h };

        if (!leftToRight)
            destRect = {WINDOW_WIDTH - surface->w - 10, offsetY, surface->w, surface->h};
        
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
        offsetY += surface->h;

        // Freeing resources
        SDL_FreeSurface(surface);   
        SDL_DestroyTexture(texture);
    }

    
}