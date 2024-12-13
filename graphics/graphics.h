#ifndef _graphics_h
#define _graphics_h

#include <SDL.h>
#include <memory>
#include <string>

class Graphics {
private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;

public:
	Graphics(int width, int height, const std::string& title);
    ~Graphics();

    // Rendering functions
    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void presentScreen();

    // Accessors
    SDL_Renderer* getRenderer() const;
};

#endif