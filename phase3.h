#ifndef _PHASE3__H
#define _PHASE3__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
#include "bullet2.h"
#include "phase2.h"
#include "laser_beam.h"

class Phase3
{
    std::vector<laserBeam> beams;
    Uint32 cooldown = 1000;
    Uint32 lastShot = 0;
    bool vertical = 1;

public:
    void generateBeam()
    {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastShot > cooldown)
        {
            int t;
            if (vertical)
                 t = generateRandom()*SCREEN_WIDTH;
            else t = generateRandom()*SCREEN_HEIGHT;
            laserBeam beam(t, vertical);
            beams.push_back(beam);
            vertical = !vertical;
            lastShot = currentTime;
        }
    }
void update()
{
    for (auto& beam : beams) {
        beam.update();
    }

    beams.erase(std::remove_if(beams.begin(), beams.end(),
                [](const laserBeam& b) { return !b.isActive(); }), beams.end());
}
    void render(const Graphics &graphics)
    {
        for (auto& beam : beams)
        {
            beam.render(graphics);
        }
    }
    bool checkPhaseCollision(const player& myPlayer)////////////////
    {
        if (myPlayer.isBlinking) return false;
        for (auto& beam : beams)
        {
            if (checkCollision(beam, myPlayer)) return true;
        }
        return false;
    }
};

inline gamePhase doPhase3(const Graphics& graphics, player& myPlayer)
{
    Sound collide(COLLIDE_SOUND_PATH);
    Sound lazer(LAZER_SOUND_PATH);
    SDL_Texture* bullet_texture = graphics.loadTexture(BULLET_FILE_PATH);
    Phase2 phase2;
    Phase3 phase3;
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) quit = true;
        if (myPlayer.isDead())
        {
            SDL_DestroyTexture(bullet_texture);
            SDL_Delay(500);
            return gamePhase::gameOver;
        }
        myPlayer.blink();
        myPlayer.moveCheck();
        phase2.generateBullet();
        phase2.update();
        if (phase2.checkPhaseCollision(myPlayer))
        {
            collide.play();
            myPlayer.loseLife();
        }

        phase3.generateBeam();
        phase3.update();
        if (phase3.checkPhaseCollision(myPlayer))
        {
            lazer.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase2.render(graphics, bullet_texture);
        SDL_SetRenderDrawColor(graphics.renderer,ENEMY_COLOR.r, ENEMY_COLOR.g, ENEMY_COLOR.b, 50);
        phase3.render(graphics);
        graphics.presentScene();
        SDL_Delay(10);
    }

    SDL_DestroyTexture(bullet_texture);
    return gamePhase::quit;
}

#endif // _PHASE3__H
