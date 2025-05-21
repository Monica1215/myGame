#ifndef _CIRCLE_BOMB__H
#define _CIRCLE_BOMB__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "player.h"

#define BOMB_RADIUS 80
#define TELEGRAPH_BOMB_TIME 1000
#define EXPANDING_BOMB_TIME 600
#define SHRINKING_BOMB_TIME 200
#define STILL_BOMB_TIME 100


#define TELEGRAPH_BOMB_PATH "img\\warning_circle.png"

enum class BombPhase
{
    Telegraph,
    Expanding,
    Still,
    Shrinking,
    Done
};

struct circleBomb
{
    int x, y;
    int radius;
    Uint32 phaseStartTime;
    BombPhase phase = BombPhase::Telegraph;
    Uint8 alpha = 255;

    circleBomb()
    {
        phaseStartTime = SDL_GetTicks();
        x = SCREEN_WIDTH*generateRandom();
        y = SCREEN_HEIGHT*generateRandom();
        radius = BOMB_RADIUS - 10;
    }

    void update()
    {
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - phaseStartTime;

        switch (phase)
        {
        case BombPhase::Telegraph:
            if (elapsed > TELEGRAPH_BOMB_TIME)
            {
                phase = BombPhase::Expanding;
                phaseStartTime = now;
            }
            break;
        case BombPhase::Expanding:
            if (elapsed > EXPANDING_BOMB_TIME)
            {
                phase = BombPhase::Still;
                phaseStartTime = now;
                alpha = 255;
                radius = BOMB_RADIUS;
            }
            else
            {
                radius = BOMB_RADIUS*1.0*elapsed/EXPANDING_BOMB_TIME;
                bool visible = (elapsed/200)%2 != 0;
                if (visible) alpha = 255; else alpha = 100;
            }
            break;
        case BombPhase::Still:
            if (elapsed > STILL_BOMB_TIME)
            {
                phase = BombPhase::Shrinking;
                phaseStartTime = now;
            }
            break;
        case BombPhase::Shrinking:
            if (elapsed > SHRINKING_BOMB_TIME)
            {
                phase = BombPhase::Done;
                phaseStartTime = now;
            }
            else
                radius = BOMB_RADIUS - BOMB_RADIUS*1.0*elapsed/SHRINKING_BOMB_TIME;
            break;
        default:
            break;
        }
    }

    void render(Texture& bullet, Texture& telegraph) const
    {
        SDL_Rect dsc = {x-radius, y-radius, 2*radius, 2*radius};

        switch (phase)
        {
        case BombPhase::Telegraph:
            telegraph.renderBasic(dsc);
            break;
        case BombPhase::Expanding:
        case BombPhase::Still:
        case BombPhase::Shrinking:
            bullet.setAlpha(alpha);
            bullet.renderBasic(dsc);
            break;
        default:
            return;

        }
    }
    bool isActive() const
    {
        return (phase != BombPhase::Done);
    }
    bool isHarmful() const
    {
        return (phase == BombPhase::Expanding || phase == BombPhase::Shrinking || phase == BombPhase::Still);
    }
};

inline bool checkCollision(const circleBomb& bullet, const player& myPlayer)
{
    if (myPlayer.isInvincible()) return false;
    if (!bullet.isHarmful()) return false;

    int closestX = std :: clamp(static_cast<int>(bullet.x), static_cast<int>(myPlayer.rect.x), static_cast<int>(myPlayer.rect.x+myPlayer.rect.w));
    int closestY = std :: clamp(static_cast<int>(bullet.y), static_cast<int>(myPlayer.rect.y), static_cast<int>(myPlayer.rect.y+myPlayer.rect.h));

    int dx = bullet.x - closestX;
    int dy = bullet.y - closestY;

    return (dx*dx + dy*dy <= bullet.radius * bullet.radius);
}

#endif // _CIRCLE_BOMB__H
