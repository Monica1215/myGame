#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>
#include "graphics.h"
#include "texture.h"
#include "font.h"

#define PLAYER_SPEED 5
#define PLAYER_SIZE 20
#define number_of_trail 15
#define BLINKING_TIME 2000
#define DASH_TIME 200
#define DASH_SPEED 15
#define FIRST_DASH 30000
#define DashCoolDown 1000


struct trailDot
{
    SDL_Rect rect;
    trailDot();
    trailDot(const SDL_Rect& Playerrect);
    void render(Uint8 alpha, const Graphics& graphics) const;
};

struct player
{
    //basic info
    SDL_Rect rect;
    int speed;
    int lives;

    //for blink
    bool isBlinking;
    Uint32 blinkStartTime;
    bool visible;

    //for effects
    std::deque<trailDot> trails;

    //for score
    Uint32 survivedTime, startCount;
    Texture score; int lastScore;
    Font gameFont;

    //for dashing
    bool isDashing = false;
    Uint32 dashStart;
    float lastVx, lastVy;
    Uint32 lastDash;


    player(Graphics &graphics);
    bool isDead();
    void loseLife();
    void render(const Graphics& graphics);
    void moveCheck();
    void blink();
    bool isInvincible() const
    {
        return (isBlinking||isDashing);
    }
    void updateTimePause(Uint32 pauseDuration)
    {
        survivedTime -= pauseDuration;
    }
    void reset(Graphics &graphics);

};






#endif // _PLAYER__H
