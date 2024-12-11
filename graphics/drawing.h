#include <SDL.h>

void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void DrawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void DrawFilledCircle2(SDL_Renderer* renderer, int centerX, int centerY, int radius);
void calculateArrowhead(float x1, float y1, float x2, float y2, float size, SDL_Vertex arrowVertices[3]);