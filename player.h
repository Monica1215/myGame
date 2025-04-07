#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>

#define PLAYER_SPEED 5
#define PLAYER_SIZE 20

struct player
{
    SDL_Rect rect;
    const int speed;
    int lives;
    bool isBlinking = 0;
    Uint32 blinkStartTime = 0;
    bool visible = 1;
    void turnNorth()
    {
        rect.y-=speed;
        rect.y = std::max(0, rect.y);
    }

    void turnSouth()
    {
        rect.y+=speed;
        rect.y = std::min(rect.y, SCREEN_HEIGHT - rect.h);
    }

    void turnWest()
    {
        rect.x-=speed;
        rect.x = std::max(rect.x, 0);
    }

    void turnEast()
    {
        rect.x+=speed;
        rect.x = std:: min(rect.x, SCREEN_WIDTH - rect.w);
    }


    player() : rect{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_SIZE, PLAYER_SIZE}, speed(PLAYER_SPEED), lives(3)
    {
    }


    bool isDead()
    {
        return (lives <= 0);
    }

    void loseLife()
    {
        isBlinking = true;
        blinkStartTime = SDL_GetTicks();
        lives--;
        if (lives == 2) rect = {rect.x, rect.y, PLAYER_SIZE/2, PLAYER_SIZE};
        if (lives == 1) rect = {rect.x, rect.y, PLAYER_SIZE/2, PLAYER_SIZE/2};
    }

    void render(const Graphics &graphics)
    {
        if (visible)
        {
        graphics.setColor(PLAYER_COLOR);
        SDL_RenderFillRect(graphics.renderer, &rect);
        graphics.setColor(BACKGROUND_COLOR);
        }
    }

    void moveCheck()
    {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) turnNorth();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) turnSouth();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) turnWest();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) turnEast();
    }

    void blink()
    {
        if (!isBlinking) return;

        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeBlinked = currentTime - blinkStartTime;

        if (timeBlinked < 2000)
        {
            visible = (timeBlinked/200)%2 != 0;
        }
        else
        {
            isBlinking = false;
            visible = true;
        }
    }
};

#endif // _PLAYER__H
