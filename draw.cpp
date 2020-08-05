#include <SDL2/SDL.h>
#include "LTexture.h"
#include <stdio.h>

SDL_Window* g_pWindow = 0;
SDL_Renderer* gRenderer = 0;
TTF_Font *gFont = 0;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

void draw();
void update();

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        //init failed
        printf("SDL_Error: %s", SDL_GetError());
        return 1;
    }

    g_pWindow = SDL_CreateWindow("Hello Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(g_pWindow != 0)
    {
        gRenderer = SDL_CreateRenderer(g_pWindow, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
    }

    TTF_Init();
    gFont = TTF_OpenFont("font.ttf", 32);
    LTexture gTexture;
    gTexture.loadRenderedText("Hello SDL", {255, 255, 255, 255});
    
    /* Alien screen coordinates */
    SDL_Rect alien;
    alien.x=50;
    alien.y=50;
    alien.w = 10;
    alien.h = 10;
    int alien_xvel=0, alien_yvel=0;
    bool quit = false;
    while(!quit)
    {
    
    
        /* Check for events */
        SDL_Event event;
        while( SDL_PollEvent( &event ) ){
            
            if(event.type == SDL_QUIT)
                quit = true;
            
            switch( event.type ){
                /* Look for a keypress */
                case SDL_KEYDOWN:
                    /* Check the SDLKey values and move change the coords */
                    switch( event.key.keysym.sym ){
                        case SDLK_LEFT:
                            alien_xvel = -1;
                            break;
                        case SDLK_RIGHT:
                            alien_xvel =  1;
                            break;
                        case SDLK_UP:
                            alien_yvel = -1;
                            break;
                        case SDLK_DOWN:
                            alien_yvel =  1;
                            break;
                        default:
                            break;
                    }
                    break;
                /* We must also use the SDL_KEYUP events to zero the x */
                /* and y velocity variables. But we must also be       */
                /* careful not to zero the velocities when we shouldn't*/
                case SDL_KEYUP:
                    switch( event.key.keysym.sym ){
                        case SDLK_LEFT:
                            /* We check to make sure the alien is moving */
                            /* to the left. If it is then we zero the    */
                            /* velocity. If the alien is moving to the   */
                            /* right then the right key is still press   */
                            /* so we don't tocuh the velocity            */
                            if( alien_xvel < 0 )
                                alien_xvel = 0;
                            break;
                        case SDLK_RIGHT:
                            if( alien_xvel > 0 )
                                alien_xvel = 0;
                            break;
                        case SDLK_UP:
                            if( alien_yvel < 0 )
                                alien_yvel = 0;
                            break;
                        case SDLK_DOWN:
                            if( alien_yvel > 0 )
                                alien_yvel = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }
        }
        
        
        /* Update the alien position */
        alien.x += alien_xvel;
        alien.y += alien_yvel;

        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        gTexture.render( (WINDOW_WIDTH-gTexture.getWidth())/2, (WINDOW_HEIGHT-gTexture.getHeight())/2);
        SDL_SetRenderDrawColor(gRenderer,255,255,255,255);
        SDL_RenderFillRect(gRenderer, &alien);
        SDL_RenderPresent(gRenderer);
    }
    SDL_Delay(1000);

    //Clean up and quit
    gTexture.free();
    TTF_Quit();
    SDL_Quit();

    return 0;
}