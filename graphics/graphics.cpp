#include "graphics.h"
#include <stdexcept>

// Custom deleters for SDL_Window and SDL_Renderer
auto windowDeleter = [](SDL_Window* ptr) { if (ptr) SDL_DestroyWindow(ptr); };
auto rendererDeleter = [](SDL_Renderer* ptr) { if (ptr) SDL_DestroyRenderer(ptr); };

Graphics::Graphics(int width, int height, const std::string& title)
    : window(nullptr, windowDeleter),
      renderer(nullptr, rendererDeleter) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    // Initialize SDL_Window
    window.reset(SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    ));
    if (!window) {
        throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
    }

    // Initialize SDL_Renderer
    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer) {
        throw std::runtime_error("Failed to create SDL Renderer: " + std::string(SDL_GetError()));
    }
}

Graphics::~Graphics() {
    SDL_Quit();
}

void Graphics::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
    SDL_RenderClear(renderer.get());
}

void Graphics::presentScreen() {
    SDL_RenderPresent(renderer.get());
}

SDL_Renderer* Graphics::getRenderer() const {
    return renderer.get();
}