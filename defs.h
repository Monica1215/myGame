#ifndef _DEFS__H
#define _DEFS__H

#define PLAYER_SPEED 5
#define PLAYER_SIZE 20
#include <SDL.h>
#include <bits/stdc++.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "Hello World!"
const SDL_Color PLAYER_COLOR = {27, 239, 255};
const SDL_Color BACKGROUND_COLOR = {0, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255, 0};

#define BULLET_FILE_PATH "img\\bullet.png"
#define FONT_PATH "assets\\PixelSansSerif.ttf"


inline float generateRandom()
{
    return (float) rand() / RAND_MAX;
}

enum class GameStates
{
    Intro,
    Playing,
    Quit,
    GameOver,
    Null,
};

#endif // _DEFS__H
