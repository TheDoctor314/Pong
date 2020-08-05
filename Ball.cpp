#include "Ball.h"

Ball::Ball(int x, int y, int width, int height)
{
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    /*do
    {
        dx = Random::get( -5, 5);
    }while( dx == 0);
    dy = Random::get<bool>() ? -4 : 4;*/
}
void Ball::update()
{
    rect.x = rect.x + dx;
    rect.y = rect.y + dy;

    if(rect.y < 0)
    {
        rect.y = 0;
        dy = -dy;
    }
    else if(rect.y > (WINDOW_HEIGHT - rect.h))
    {
        rect.y = WINDOW_HEIGHT - rect.h;
        dy = -dy;
    }


}
void Ball::render(SDL_Renderer* RenderObj)
{
    SDL_RenderFillRect(RenderObj, &rect);
}
bool Ball::collides(Paddle* obj)
{
    SDL_Rect box = obj->rect;

    if(( rect.x > (box.x + box.w) )||( box.x > (rect.x + rect.w) ))
        return false;
    
    if(( rect.y  > (box.y + box.h) )||( box.y > (rect.y + rect.h) ))
        return false;
    
    return true;
}
void Ball::reset(int xvel, int yvel)
{
    rect.x = (WINDOW_WIDTH - rect.w) / 2;
    rect.y = (WINDOW_HEIGHT - rect.h) / 2;
    dx = xvel;
    dy = yvel;
}