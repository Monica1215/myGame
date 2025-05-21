#ifndef _PHASE1__H
#define _PHASE1__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
#include "phaseQuit.h"
struct Phase1
{
    std::vector<Bullet> bullets;
    Uint32 cooldown;
    Uint32 lastShot = 0;
    Uint32 lastCount = 0;
    Uint32 phaseTime = 0;

    Texture bullet_texture;

    Phase1(const Graphics &graphics, int &gameLoop): bullet_texture(graphics.renderer)
    {
        bullet_texture.loadFromFile(BULLET_FILE_PATH);
        lastCount = SDL_GetTicks();
        cooldown = 500 - gameLoop*50;
    }
    void generateBullet()
    {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastShot > cooldown)
        {
            Bullet bullet(0, generateRandom()*SCREEN_WIDTH, 20, 0.0);
            bullets.push_back(bullet);
            lastShot = currentTime;
        }
    }

    void update()
    {
        Uint32 current = SDL_GetTicks();
        phaseTime += current-lastCount;
        lastCount = current;

        for (auto& bullet : bullets)
        {
            bullet.update();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return !b.isActive(); }), bullets.end());

    }
    void render() const
    {
        for (auto& bullet : bullets)
        {
            bullet.render(bullet_texture);
        }
    }

    void updateTimePause(Uint32 pauseDuration)
    {
        lastShot += pauseDuration;
        phaseTime -= pauseDuration;
    }

    bool checkPhaseCollision(const player& myPlayer)
    {
        for (auto& bullet : bullets)
        {
            if (!myPlayer.isBlinking &&checkCollision(bullet, myPlayer))
                return true;
        }
        return false;
    }

    bool endPhase()
    {
        return (phaseTime > PHASE_TIME);
    }
};

inline gamePhase doPhase1(Graphics& graphics, player& myPlayer, int &gameLoop)
{

    Phase1 phase1(graphics, gameLoop);
    SDL_Event e;
    Sound collide(COLLIDE_SOUND_PATH);
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
                phase1.updateTimePause(pauseDuration);
                myPlayer.updateTimePause(pauseDuration);
            }
        if (myPlayer.isDead())
        {
            SDL_Delay(10);
            return gamePhase::gameOver;
        }
        myPlayer.moveCheck();
        phase1.generateBullet();
        phase1.update();
        if (phase1.checkPhaseCollision(myPlayer))
        {
            if (graphics.sound) collide.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase1.render();
        graphics.presentScene();
        if (phase1.endPhase())
        {
            return gamePhase::Phase2;
        }
        SDL_Delay(10);
    }

    return gamePhase::quit;
}

#endif // _PHASE1__
