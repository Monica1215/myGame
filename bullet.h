#ifndef _BULLET__H
#define _BULLET__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "player.h"
#include "texture.h"

#define INITIAL_BULLET_SPEED 3

struct Bullet
{
    float x, y;
    int radius;
    double angle;
    int speed = INITIAL_BULLET_SPEED;

    Bullet()
    {
        x = 0; y = 0; radius = 0; angle = 0;
    }

    Bullet(float _x, float _y, int r, double _angle )
    {
        x = _x;
        y = _y;
        radius = r;
        angle = _angle;
    }

    void render(Texture &bullet_texture,const Graphics &graphics) const
    {
        SDL_Rect dsc;
        dsc.x = static_cast<int>(x) - radius;
        dsc.y = static_cast<int>(y) - radius;
        dsc.w = 2*radius;
        dsc.h = 2*radius;
        bullet_texture.renderBasic(dsc);
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
    int closestX = std :: clamp(static_cast<int>(bullet.x), static_cast<int>(myPlayer.rect.x), static_cast<int>(myPlayer.rect.x+myPlayer.rect.w));
    int closestY = std :: clamp(static_cast<int>(bullet.y), static_cast<int>(myPlayer.rect.y), static_cast<int>(myPlayer.rect.y+myPlayer.rect.h));

    int dx = bullet.x - closestX;
    int dy = bullet.y - closestY;

    return (dx*dx + dy*dy <= bullet.radius * bullet.radius);
}

#endif // _BULLET__H
