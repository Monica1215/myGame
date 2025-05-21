#ifndef _PHASE_TRANSITION__H
#define _PHASE_TRANSITION__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"

#define RIPPLE_DURATION 2000
#define TRANSITION_DURATION 2000
#define RIPPLE_COOLDOWN 500
#define RIPPLE_PATH "img\\ripple.png"
class Ripple
{
    int x, y;
    int radius;
    Uint8 alpha;
    Uint32 startTime = 0;
public:
    Ripple(int _x, int _y )
    {
        x = _x;
        y = _y;
        radius = 0;
        alpha = 255;
        startTime = SDL_GetTicks();
    }
    bool isAlive() const
    {
        return (SDL_GetTicks()-startTime <= RIPPLE_DURATION);
    }

    void update()
    {
        if (!isAlive()) return;
        Uint32 elapsed = SDL_GetTicks() - startTime;
        float t = 1.0*elapsed/RIPPLE_DURATION;
        radius = SCREEN_WIDTH*t;
        alpha = 255*(1-t);
    }
    void render(Texture &texture) const
    {
        if (!isAlive()) return;
        texture.setAlpha(alpha);
        SDL_Rect dsc = {x-radius, y-radius, 2*radius, 2*radius};
        texture.renderBasic(dsc);
    }
};

inline gamePhase doPhaseTransition(const Graphics &graphics, player &myPlayer, gamePhase current)
{
    if (myPlayer.isDead()) return gamePhase::gameOver;
    if (current == gamePhase::quit) return current;
    Uint32 startTime = SDL_GetTicks();
    std::vector<Ripple> ripples;
    Uint32 lastShot = 0;
    Uint32 currentShot = SDL_GetTicks();
    Texture ripple_t(graphics.renderer);
    ripple_t.loadFromFile(RIPPLE_PATH);
    ripple_t.setBlendMode(SDL_BLENDMODE_BLEND);
    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        graphics.prepareScene();

        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
            {
                quit = true;
                return gamePhase::quit;
            }
        //generate
        currentShot = SDL_GetTicks();
        if (currentShot - lastShot > RIPPLE_COOLDOWN)
        {
            Ripple r(0, SCREEN_HEIGHT/2);
            //Ripple r(myPlayer.rect.x + myPlayer.rect.w/2, myPlayer.rect.y + myPlayer.rect.h/2);
            ripples.push_back(r);
            lastShot = currentShot;
        }

        for (auto &r : ripples ) r.update();
        myPlayer.blink();
        myPlayer.moveCheck();


        for (auto &r : ripples) r.render(ripple_t);
        myPlayer.render(graphics);
        graphics.presentScene();

        SDL_Delay(10);
        if (currentShot - startTime > TRANSITION_DURATION) return current;
    }
    return current;
}

#endif // _PHASE_TRANSITION__H
