#include "drawing.h"

void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x; // Decision criterion

    while (y <= x) {
        // Draw each octant of the circle
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        y++;

        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1; // Change in decision criterion for y -> y+1
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1; // Change for y -> y+1, x -> x-1
        }
    }
}

void DrawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

// Aparentemente este algoritmo é mais rápido
void DrawFilledCircle2(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x; // Decision criterion

    while (y <= x) {
        // Draw horizontal lines between symmetric points
        SDL_RenderDrawLine(renderer, centerX - x, centerY + y, centerX + x, centerY + y);
        SDL_RenderDrawLine(renderer, centerX - x, centerY - y, centerX + x, centerY - y);
        SDL_RenderDrawLine(renderer, centerX - y, centerY + x, centerX + y, centerY + x);
        SDL_RenderDrawLine(renderer, centerX - y, centerY - x, centerX + y, centerY - x);

        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1; // Update for moving vertically
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1; // Update for moving diagonally
        }
    }
}

void calculateArrowhead(float x1, float y1, float x2, float y2, float size, SDL_Vertex arrowVertices[3]) {
    // Calculate direction vector
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrt(dx * dx + dy * dy);

    // Normalize direction vector
    dx /= length;
    dy /= length;

    // Perpendicular vectors for the base of the triangle
    float perpX = -dy * size * 0.5f;
    float perpY = dx * size * 0.5f;

    // Arrowhead vertices
    SDL_Vertex v0 = { {x2, y2}, {255, 255, 255, 255}, {0.0f, 0.0f} };  // Tip of the arrow
    SDL_Vertex v1 = { {x2 - dx * size + perpX, y2 - dy * size + perpY}, {255, 255, 255, 255}, {0.0f, 0.0f} };  // Left base
    SDL_Vertex v2 = { {x2 - dx * size - perpX, y2 - dy * size - perpY}, {255, 255, 255, 255}, {0.0f, 0.0f} };  // Right base

    arrowVertices[0] = v0;
    arrowVertices[1] = v1;
    arrowVertices[2] = v2;

    // arrowVertices[0] = {{x2, y2}, {255, 0, 0, 255}, {0.0f, 0.0f}}; // Tip of the arrow
    // arrowVertices[1] = {{x2 - dx * size + perpX, y2 - dy * size + perpY}, {255, 0, 0, 255}, {0.0f, 0.0f}}; // Left base
    // arrowVertices[2] = {{x2 - dx * size - perpX, y2 - dy * size - perpY}, {255, 0, 0, 255}, {0.0f, 0.0f}}; // Right base
}