#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include "globals.h"
#include "Ball.h"
#include "Paddle.h"
#include "LTexture.h"

SDL_Window* gWindow = 0;
SDL_Renderer* gRenderer = 0;
TTF_Font* gFont = 0;

Ball* ball = 0;
Paddle* paddle1 = 0;
Paddle* paddle2 = 0;

LTexture start, players, serve, score, done;
SDL_Color White = {255, 255, 255, 255};
SDL_Rect option_dot;
SDL_Point pos1, pos2;

#define COMPUTER 1
#define HUMAN 0

int player1score, player2score, servingPlayer, winningPlayer = 0, winningScore = 3, playerState;
GameStates gameState;
bool quit = false;

bool init();
void load();
void gameProcessKeypress(SDL_Event *event);
void draw();
void update();
void displayScore();
void close();

int main(int argc, char *argv[])
{
    if( init() )
    {
        printf("Game load failed\n");
        return 1;
    }
    load();

    SDL_Event event;
    while(!quit)
    {
        gameProcessKeypress(&event);
        update();
        draw();
    }

    close();
    return 0;
}

//returns true in case of failure
bool init()
{
    bool failure = false;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        //init failed
        printf("SDL_Error: %s\n", SDL_GetError());
        failure = true;
    }

    gWindow = SDL_CreateWindow("Hello Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    if(gWindow != 0)
    {
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
    if(gRenderer == 0)
        failure = true;

    if(TTF_Init() < 0)
    {
        printf("TTF_Error: %s", TTF_GetError());
        failure = true;
    }

    gFont = TTF_OpenFont("font.ttf", 32);
    if(gFont == NULL)
    {
        printf("Failed to load font. TTF error: %s", TTF_GetError());
        failure = true;
    }
    
    return failure;
}
void load()
{
    player1score = 0;
    player2score = 0;
    servingPlayer = 1;
    gameState = GAME_START;

    paddle1 = new Paddle(5, 50, 15, 60);
    paddle2 = new Paddle(WINDOW_WIDTH-20, WINDOW_HEIGHT-110, 15, 60);
    ball = new Ball((WINDOW_WIDTH - 12)/2, (WINDOW_HEIGHT - 12)/2, 12, 12);

    start.loadRenderedText("Press Enter to start", White);
    serve.loadRenderedText("Press Enter to serve", White);
    players.loadRenderedText("1 Player\n2 Players", White);

    pos1.x = (WINDOW_WIDTH - players.getWidth())/2;
    pos2.x = pos1.x;
    pos1.y = 200;
    pos2.y = 250;
    option_dot.w = option_dot.h = 12;
    option_dot.x = pos1.x;
    option_dot.y = pos1.y;
}
void gameProcessKeypress(SDL_Event *event)
{
    while( SDL_PollEvent(event) )
    {
        if( (event->type == SDL_QUIT) )
            quit = true;
        else if( event->type == SDL_KEYDOWN )
        {
            switch (event->key.keysym.scancode) 
            {
                case SDL_SCANCODE_ESCAPE:
                    quit = true;
                    break;
                case SDL_SCANCODE_RETURN:
                    if( gameState == GAME_START)
                    {
                        gameState = GAME_SERVE;
                        playerState = (option_dot.y == pos1.y) ? COMPUTER : HUMAN;
                    }
                    else if( gameState == GAME_SERVE)
                        gameState = GAME_PLAY;
                    else if( gameState == GAME_DONE)
                    {
                        gameState = GAME_SERVE;
                        ball->reset(0, 0);

                        if(winningPlayer == 1)
                            servingPlayer = 2;
                        else
                            servingPlayer = 1;
                        
                        player1score = 0;
                        player2score = 0;
                        
                    }
                    break;
                //player 1 movement
                case SDL_SCANCODE_W:
                    paddle1->dy = -PADDLE_SPEED;
                    break;
                case SDL_SCANCODE_S:
                    paddle1->dy = PADDLE_SPEED;
                    break;
                //player 2 movement
                case SDL_SCANCODE_UP:
                    paddle2->dy = -PADDLE_SPEED;
                    break;
                case SDL_SCANCODE_DOWN:
                    paddle2->dy = PADDLE_SPEED;
                    break;
                //select options
                case SDL_SCANCODE_1:
                    option_dot.x = pos1.x;
                    option_dot.y = pos1.y;
                    break;
                case SDL_SCANCODE_2:
                    option_dot.x = pos2.x;
                    option_dot.y = pos2.y;
                    break;
                default:
                    break;
            }
        }
        else if(event->type == SDL_KEYUP)
        {
            switch (event->key.keysym.scancode) 
            {
                //player 1 movement
                case SDL_SCANCODE_W:
                    if( paddle1->dy< 0)
                        paddle1->dy = 0;
                    break;
                case SDL_SCANCODE_S:
                    if( paddle1->dy > 0)
                        paddle1->dy = 0;
                    break;
                //player 2 movement
                case SDL_SCANCODE_UP:
                    if( paddle2->dy < 0)
                        paddle2->dy = 0;
                    break;
                case SDL_SCANCODE_DOWN:
                    if( paddle2->dy > 0)
                        paddle2->dy = 0;
                    break;
                default:
                    break;
            } 
        }

    }
}
void draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    ball->render(gRenderer);
    paddle1->render(gRenderer);
    paddle2->render(gRenderer);

    if(gameState == GAME_START)
    {
        start.render( (WINDOW_WIDTH - start.getWidth())/2, 100 );
        players.render( WINDOW_WIDTH/2, 200);
        SDL_RenderFillRect(gRenderer, &option_dot);
    }
    else if (gameState == GAME_SERVE)
    {
        serve.render( (WINDOW_WIDTH - serve.getWidth())/2, 200 );
        displayScore();
    }
    else if(gameState == GAME_DONE)
    {
        std::stringstream winMessage;
        winMessage<<"Player "<<winningPlayer<<" wins!";
        done.loadRenderedText( winMessage.str(), White);
        done.render( (WINDOW_WIDTH - done.getWidth())/2, (WINDOW_HEIGHT - done.getHeight())/2 );
    }
    
    SDL_RenderPresent(gRenderer);
}
void update()
{
    if(gameState == GAME_PLAY)
    {
        if( ball->collides(paddle1) )
        {
            ball->dx = -ball->dx;
            ball->rect.x = paddle1->rect.x + paddle1->rect.w;

            if( ball->dy > 0)
                ball->dy = Random::get(-10, 10);
            else
                ball->dy = -Random::get(-10, 10);
        }
        if( ball->collides(paddle2) )
        {
            ball->dx = -ball->dx;
            ball->rect.x = paddle2->rect.x - ball->rect.w;

            if( ball->dy > 0)
                ball->dy = Random::get(-10, 10);
            else
                ball->dy = -Random::get(-10, 10);
        }

        if( ball->rect.x <= 0)
        {
            player2score++;
            servingPlayer = 1;

            if( player2score == winningScore)
            {
                winningPlayer = 2;
                gameState = GAME_DONE;
            }
            else
            {
                gameState = GAME_SERVE;
                ball->reset(0, 0);
            }  
        }
        else if( ball->rect.x >=WINDOW_WIDTH)
        {
            player1score++;
            servingPlayer = 2;

            if( player1score == winningScore)
            {
                winningPlayer = 1;
                gameState = GAME_DONE;
            }
            else
            {
                gameState = GAME_SERVE;
                ball->reset(0, 0);
            }
        }

        if(playerState == COMPUTER)
        {
            if(ball->rect.y < (paddle2->rect.y + (paddle2->rect.h)/2))
                paddle2->dy = -PADDLE_SPEED;
            if(ball->rect.y > (paddle2->rect.y + (paddle2->rect.h)/2))
                paddle2->dy = PADDLE_SPEED;
        }
    }
    else if (gameState == GAME_SERVE)
    {
        if(servingPlayer == 1)
        {
            ball->dx = Random::get(3, 7);
        }
        else
        {
            ball->dx = -Random::get(3, 7);
        }
    }
    
    if(gameState == GAME_PLAY)
        ball->update();
    paddle1->update();
    paddle2->update();
}
void displayScore()
{
    std::stringstream msg;
    msg<<player1score<<" - "<<player2score;
    score.loadRenderedText(msg.str(), White);
    score.render( (WINDOW_WIDTH - score.getWidth())/2, 50);
}
void close()
{
    start.free();
    serve.free();
    score.free();
    done.free();
    TTF_CloseFont(gFont);
    delete ball;
    delete paddle1;
    delete paddle2;
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
}