#ifndef _BULLET2__H
#define _BULLET2__H
#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"

#define EXISTING_TIME 5000
#define BIGSIZE 20
#define SMALLSIZE 5
struct BulletType2
{
    Bullet bigBullet;
    std::vector<Bullet> smallBullet;
    int destinationBigBullet;
    bool visible = 1;
    Uint32 startTime = 0;
    bool down;
    bool exploded = false;
    BulletType2(bool _down) : down(_down)
    {
        startTime = SDL_GetTicks();
        if (down)
        {
            bigBullet = Bullet(generateRandom()*SCREEN_WIDTH, 0, BIGSIZE, M_PI/2);
            destinationBigBullet = SCREEN_HEIGHT/3;
        }
        else
        {
            bigBullet = Bullet(generateRandom()*SCREEN_WIDTH, SCREEN_HEIGHT, BIGSIZE, 3*M_PI/2);
            destinationBigBullet = 2*SCREEN_HEIGHT/3;
        }

    }

    void update()
    {
        if (!exploded)
        {
            if ((down && bigBullet.y < destinationBigBullet) || (!down && bigBullet.y > destinationBigBullet))
            {
                bigBullet.update();
            }
            else
            {
                exploded = true;
                visible = false;
                for (int i = -4; i < 4; i++)
                {
                    double angle = i*M_PI/4;
                    Bullet b(bigBullet.x, bigBullet.y, SMALLSIZE, angle);
                    smallBullet.push_back(b);
                }
                for (auto &i : smallBullet) i.update();
            }
        }
        else
        {
            for (auto &b : smallBullet)
                b.update();
        }

    }

    void render(SDL_Texture* texture,const Graphics &graphics) const
    {
        if (visible)
        {
            bigBullet.render(texture, graphics);
        }
        else
        {
            for (auto &b:smallBullet)
                b.render(texture, graphics);
        }
    }

    bool isActive() const
    {
        Uint32 currentTime = SDL_GetTicks();
        return (currentTime - startTime <= EXISTING_TIME);
    }
};

#endif // _BULLET2__H
