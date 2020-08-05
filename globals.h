#ifndef GLOBALS_H
#define GLOBALS_H

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const int PADDLE_SPEED = 15;

enum GameStates
{
    GAME_START,
    GAME_SERVE,
    GAME_PLAY,
    GAME_DONE
};

extern int player1score, player2score, servingPlayer;
extern GameStates gameState;

#endif