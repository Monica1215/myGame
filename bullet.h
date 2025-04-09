#ifndef _BULLET__H
#define _BULLET__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "player.h"

#define INITIAL_BULLET_SPEED 3

struct Bullet
{
    int x, y;
    int radius;
    double angle;
    int speed = INITIAL_BULLET_SPEED;

    Bullet(int _x, int _y, int r, double _angle )
    {
        x = _x;
        y = _y;
        radius = r;
        angle = _angle;
    }

    void render(SDL_Texture* texture,const Graphics &graphics) const
    {
        SDL_Rect dsc;
        dsc.x = x - radius;
        dsc.y = y - radius;
        dsc.w = 2*radius;
        dsc.h = 2*radius;
        SDL_RenderCopy(graphics.renderer, texture, NULL, &dsc);
    }
    void update()
    {
        x += speed*cos(angle);
        y += speed*sin(angle);
    }

    bool isActive() const
    {
        return (x>=0 && x<=SCREEN_WIDTH && y>=0 && y<=SCREEN_HEIGHT);
    }
};

inline bool checkCollision(const Bullet& bullet, const player& myPlayer)
{
    int closestX = std :: clamp(bullet.x, myPlayer.rect.x, myPlayer.rect.x+myPlayer.rect.w);
    int closestY = std :: clamp(bullet.y, myPlayer.rect.y, myPlayer.rect.y+myPlayer.rect.h);

    int dx = bullet.x - closestX;
    int dy = bullet.y - closestY;

    return (dx*dx + dy*dy <= bullet.radius * bullet.radius);
}

#endif // _BULLET__H
