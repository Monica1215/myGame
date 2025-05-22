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
            Ripple r(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, RIPPLE_DURATION);
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
