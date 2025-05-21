#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>
#include "graphics.h"

#define PLAYER_SPEED 5
#define PLAYER_SIZE 20
#define number_of_trail 15
#define BLINKING_TIME 2000
struct trailDot
{
    SDL_Rect rect;
    trailDot();
    trailDot(const SDL_Rect& Playerrect);
    void render(Uint8 alpha, const Graphics& graphics) const;
};

struct player
{
    SDL_Rect rect;
    int speed;
    int lives;
    bool isBlinking;
    Uint32 blinkStartTime;
    bool visible;
    std::deque<trailDot> trails;
    Uint32 survivedTime, startCount;
    void turnNorth();
    void turnSouth();
    void turnWest();
    void turnEast();
    player();
    bool isDead();
    void loseLife();
    void render(const Graphics& graphics);
    void moveCheck();
    void blink();
    void updateTimePause(Uint32 pauseDuration)
    {
        survivedTime -= pauseDuration;
    }
    void reset()
    {
        *this = player();
    }
};






#endif // _PLAYER__H
