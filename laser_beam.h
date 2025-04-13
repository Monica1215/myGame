#ifndef _LASER_BEAM__H
#define _LASER_BEAM__H


#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "player.h"
#define LASER_WIDTH 10
#define TELEGRAPH_TIME 1000
#define ACTIVE_TIME 1000
#define FADEOUT_TIME 800

enum class BeamPhase
{
    Telegraph,
    Active,
    FadeOut,
    Done
};

struct laserBeam
{
    SDL_Rect rect;
    Uint32 phaseStartTime = 0;
    bool vertical = true;
    BeamPhase phase = BeamPhase::Telegraph;
    Uint8 currentAlpha = 0;

    laserBeam(int t, bool _vertical)
    {
        vertical = _vertical;
        phaseStartTime = SDL_GetTicks();

        if (vertical)
        {
            rect = { t, 0, LASER_WIDTH, SCREEN_HEIGHT };
        }
        else
        {
            rect = { 0, t, SCREEN_WIDTH, LASER_WIDTH };
        }
    }

    void update()
    {
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - phaseStartTime;

        switch (phase)
        {
            case BeamPhase::Telegraph:
                if (elapsed > TELEGRAPH_TIME)
                {
                    phase = BeamPhase::Active;
                    phaseStartTime = now;
                }
                break;

            case BeamPhase::Active:
                if (elapsed > ACTIVE_TIME)
                {
                    phase = BeamPhase::FadeOut;
                    phaseStartTime = now;
                }
                break;

            case BeamPhase::FadeOut:
                if (elapsed > FADEOUT_TIME)
                {
                    phase = BeamPhase::Done;
                }
                else
                {
                    currentAlpha = 255 - static_cast<Uint8>(255.0 * elapsed / FADEOUT_TIME);
                }
                break;
            default:
                break;
        }
    }

    void render(const Graphics& graphics)
    {
        switch (phase)
        {
            case BeamPhase::Telegraph:
                SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 120);
                break;

            case BeamPhase::Active:
                SDL_SetRenderDrawColor(graphics.renderer, ENEMY_COLOR.r, ENEMY_COLOR.g, ENEMY_COLOR.b, 255);
                break;

            case BeamPhase::FadeOut:
                SDL_SetRenderDrawColor(graphics.renderer, ENEMY_COLOR.r, ENEMY_COLOR.g, ENEMY_COLOR.b, currentAlpha);
                break;

            default:
                return;
        }

        SDL_RenderFillRect(graphics.renderer, &rect);
    }

    bool isActive() const
    {
        return phase != BeamPhase::Done;
    }

    bool isHarmful() const
    {
        return phase == BeamPhase::Active;
    }
};

inline bool checkCollision(const laserBeam& beam, const player& myPlayer)
{
    if (!beam.isHarmful()) return false;

    return SDL_HasIntersection(&beam.rect, &myPlayer.rect);
}
#endif // _LASER_BEAM__H
