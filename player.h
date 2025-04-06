#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>

struct movingObject
{
    SDL_Rect rect;
    int speed;
    void setSpeed(int _speed)
    {
        speed = _speed;
    }

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

    void setObject(int x, int y, int w, int h)
    {
        rect.x=x;
        rect.y=y;
        rect.w=w;
        rect.h=h;
    }

    void render(SDL_Texture* texture,const Graphics& graphics)
    {
        SDL_RenderCopy(graphics.renderer, texture, NULL, &rect);
    }
};

struct player
{
    movingObject myPlayer;
    int lives = 3;

    void init()
    {

        myPlayer.setObject(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_SIZE, PLAYER_SIZE);
        myPlayer.setSpeed(PLAYER_SPEED);
    }


    bool isDead()
    {
        return (lives <= 0);
    }

    void loseLife()
    {
        lives--;
        if (lives == 2) myPlayer.setObject(myPlayer.rect.x, myPlayer.rect.y, PLAYER_SIZE/2, PLAYER_SIZE);
        if (lives == 1) myPlayer.setObject(myPlayer.rect.x, myPlayer.rect.y, PLAYER_SIZE/2, PLAYER_SIZE/2);
    }

    void render(const Graphics &graphics)
    {
        graphics.setColor(PLAYER_COLOR);
        SDL_RenderFillRect(graphics.renderer, &myPlayer.rect);
        graphics.setColor(BACKGROUND_COLOR);
    }

};

#endif // _PLAYER__H
