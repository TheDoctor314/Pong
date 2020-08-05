#include "Paddle.h"

Paddle::Paddle(int x, int y, int width, int height)
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
}
void Paddle::update()
{
    rect.y = rect.y + dy;

    if(rect.y < 0)
        rect.y = 0;
    else if(rect.y > (WINDOW_HEIGHT - rect.h))
        rect.y = WINDOW_HEIGHT - rect.h;

}
void Paddle::render(SDL_Renderer* RenderObj)
{
    SDL_RenderFillRect(RenderObj, &rect);
}