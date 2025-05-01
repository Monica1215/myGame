#ifndef _PHASE4__H
#define _PHASE4__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
#include "bullet2.h"
#include "phase2.h"
#include "laser_beam.h"
#include "texture.h"
#include "circle_bomb.h"


struct Phase4
{
    std :: vector<circleBomb> bombs;
    Uint32 cooldown = 1000;
    Uint32 lastShot = 0;
    Uint32 lastCount = 0;
    Uint32 phaseTime = 0;

    Texture bullet_texture;
    Texture telegraph_texture;

    Phase4(const Graphics &graphics): bullet_texture(graphics.renderer), telegraph_texture(graphics.renderer)
    {
        bullet_texture.loadFromFile(BULLET_FILE_PATH);
        telegraph_texture.loadFromFile(TELEGRAPH_BOMB_PATH);

        lastCount = SDL_GetTicks();
    }

    void generateBullet()
    {
        Uint32 now = SDL_GetTicks();
        if (now - lastShot > cooldown)
        {
            circleBomb b;
            bombs.push_back(b);
            lastShot = now;
        }
    }

    void update()
    {
        Uint32 current = SDL_GetTicks();
        phaseTime += current-lastCount;
        lastCount = current;

        for (auto &bomb: bombs)
        {
            bomb.update();
        }

        bombs.erase(std::remove_if(bombs.begin(), bombs.end(),
                    [](const circleBomb& b) { return !b.isActive(); }), bombs.end());
    }

    void render() const
    {
        for (auto &b : bombs)
        {
            b.render(bullet_texture, telegraph_texture);
        }
    }

    void updateTimePause(Uint32 pauseDuration)
    {
        lastShot += pauseDuration;
        phaseTime -= pauseDuration;
    }

    bool checkPhaseCollision(const player& myPlayer)
    {
        if (myPlayer.isBlinking) return false;
        for (auto& b : bombs)
        {
            if (checkCollision(b, myPlayer)) return true;
        }
        return false;
    }

    bool endPhase()
    {
        return (phaseTime > PHASE_TIME);
    }
};

inline gamePhase doPhase4(Graphics& graphics, player& myPlayer)
{
    Sound collide(COLLIDE_SOUND_PATH);
    Phase4 phase4(graphics);
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
                phase4.updateTimePause(pauseDuration);
                myPlayer.updateTimePause(pauseDuration);
            }
        if (myPlayer.isDead())
        {
            SDL_Delay(500);
            return gamePhase::gameOver;
        }
        myPlayer.moveCheck();
        phase4.generateBullet();
        phase4.update();
        if (phase4.checkPhaseCollision(myPlayer))
        {
            if (graphics.sound) collide.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase4.render();
        graphics.presentScene();
        if (phase4.endPhase())
        {
            return gamePhase::Phase1;
        }
        SDL_Delay(10);
    }

    return gamePhase::quit;
}

#endif // _PHASE4__H
