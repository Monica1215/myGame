#ifndef _DEFS__H
#define _DEFS__H

#define PLAYER_SPEED 5
#define PLAYER_SIZE 20
#include <SDL.h>
#include <bits/stdc++.h>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Hello World!";
const SDL_Color PLAYER_COLOR = {27, 239, 255};
const SDL_Color BACKGROUND_COLOR = {0, 0, 0};
const char* PLAYER_FILE_PATH = "img\\player.png";
const char* BULLET_FILE_PATH = "img\\bullet.png";

float generateRandom()
{
    return (float) rand() / RAND_MAX;
}

#endif // _DEFS__H
