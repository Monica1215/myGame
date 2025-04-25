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

struct Phase3
{
    std::vector<laserBeam> beams;
    Uint32 cooldown = 500;
    Uint32 lastShot = 0;
    Uint32 lastCount = 0;
    Uint32 phaseTime = 0;
    Phase2 phase2;
    bool vertical = 1;

    Phase3(const Graphics &graphics) : phase2(graphics)
    {
        lastCount = SDL_GetTicks();
    }
    void generateBullet()
    {
        //generate beam
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

        // generate phase 2 bullet
        phase2.generateBullet();
    }
    void update()
    {
        Uint32 current = SDL_GetTicks();
        phaseTime += current-lastCount;
        lastCount = current;

        for (auto& beam : beams) {
        beam.update();
        }

        beams.erase(std::remove_if(beams.begin(), beams.end(),
                [](const laserBeam& b) { return !b.isActive(); }), beams.end());

        phase2.update();
    }

    void render(const Graphics &graphics)
    {

        SDL_SetRenderDrawColor(graphics.renderer,ENEMY_COLOR.r, ENEMY_COLOR.g, ENEMY_COLOR.b, 50);
        for (auto& beam : beams)
        {
            beam.render(graphics);
        }
        phase2.render();
    }

    void updateTimePause(Uint32 pauseDuration)
    {
        lastShot += pauseDuration;
        phaseTime -= pauseDuration;
        phase2.updateTimePause(pauseDuration);
    }

    bool checkPhaseCollision(const player& myPlayer)////////////////
    {
        if (myPlayer.isBlinking) return false;
        for (auto& beam : beams)
        {
            if (checkCollision(beam, myPlayer)) return true;
        }
        return (phase2.checkPhaseCollision(myPlayer));
    }
};

inline gamePhase doPhase3(Graphics& graphics, player& myPlayer)
{
    Sound collide(COLLIDE_SOUND_PATH);
    Sound lazer(LAZER_SOUND_PATH);
    Texture bullet_texture(graphics.renderer);
    bullet_texture.loadFromFile(BULLET_FILE_PATH);
    Phase3 phase3(graphics);
    SDL_Event e;
    while (true)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
            {
                Uint32 pauseStart = SDL_GetTicks();
                if (graphics.music) graphics.mus.pause();
                quitRespond res = doPhaseQuit(graphics);

                if (res == quitRespond::quit) return gamePhase::quit;
                if (graphics.music) graphics.mus.play();

                Uint32 pauseDuration = SDL_GetTicks() - pauseStart;
                phase3.phaseTime -=  pauseDuration;
                phase3.updateTimePause(pauseDuration);
                myPlayer.updateTimePause(pauseDuration);
            }
        if (myPlayer.isDead())
        {
            SDL_Delay(500);
            return gamePhase::gameOver;
        }
        myPlayer.moveCheck();

        phase3.generateBullet();
        phase3.update();
        if (phase3.checkPhaseCollision(myPlayer))
        {
            if(graphics.sound) lazer.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase3.render(graphics);
        graphics.presentScene();
        SDL_Delay(10);
    }

    return gamePhase::quit;
}

#endif // _PHASE3__H
