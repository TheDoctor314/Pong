#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>
#include "globals.h"
class Paddle
{
    public:
    SDL_Rect rect;
    int dy;

    Paddle(int x, int y, int width, int height);
    void update();
    void render(SDL_Renderer* RenderObj);
};

#endif