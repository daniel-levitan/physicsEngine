#ifndef _text_h
#define _text_h

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "../vector/vector2.h"
#include "helper.h"

class Text {
private:
    TTF_Font* font;
    std::string message;
    Vector2 position;
    SDL_Color color;
    bool leftToRight;
   
public:
    Text(const std::string& fontPath, const std::string messageText, int fontSize, Vector2 position, SDL_Color color);
    ~Text();

    void setMessage(const std::string messageText) {
        message = messageText;
    }
    void setPosition(Vector2 newPosition) {
        position = newPosition;
    }
    void setColor(SDL_Color newColor) {
        color = newColor;
    }

    void setDirectionRightToLeft() {
        leftToRight = 0; 
    }

    void setDirectionLeftToRight() {
        leftToRight = 1; 
    }

    void render(SDL_Renderer* renderer);    // , SDL_Color color, int x, int y);
};

#endif