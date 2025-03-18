#ifndef _PLAYER__H
#define _PLAYER__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"

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
    }

    void turnSouth()
    {
        rect.y+=speed;
    }

    void turnWest()
    {
        rect.x-=speed;
    }

    void turnEast()
    {
        rect.x+=speed;
    }

    void setObject(int x, int y, int w, int h)
    {
        rect.x=x;
        rect.y=y;
        rect.w=w;
        rect.h=h;
    }

    void render(SDL_Texture* texture, Graphics graphics)
    {
        SDL_RenderCopy(graphics.renderer, texture, NULL, &rect);
    }
};

struct player
{
    movingObject myPlayer;
    int lives = 3;
    SDL_Texture* texture;

    void init_and_loadTexture(const char* path, Graphics graphics)
    {
        texture = graphics.loadTexture(path);
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

    void render(Graphics graphics)
    {
        myPlayer.render(texture, graphics);
    }

    void quit()
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
};

#endif // _PLAYER__H
