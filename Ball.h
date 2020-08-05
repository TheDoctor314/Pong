#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include "globals.h"
#include "Paddle.h"
#include "random.hpp"

using Random = effolkronium::random_static;

class Ball
{
    public:
    SDL_Rect rect;
    int dx, dy;

    Ball(int x, int y, int width, int height);
    void reset(int xvel, int yvel);
    void update();
    void render(SDL_Renderer* RenderObj);
    bool collides( Paddle* obj);//parameter should be pointer to paddle
};

#endif