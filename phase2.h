#ifndef _PHASE2__H
#define _PHASE2__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
#include "bullet2.h"

struct Phase2
{
    std::vector<BulletType2> bullets;
    Uint32 cooldown = 1000;
    Uint32 lastShot = 0;
    Uint32 lastCount = 0;
    Uint32 phaseTime = 0;

    Texture bullet_texture;

    bool down = 1;

public:
    Phase2(const Graphics &graphics) : bullet_texture(graphics.renderer)
    {
        bullet_texture.loadFromFile(BULLET_FILE_PATH);
        lastCount = SDL_GetTicks();
    }
    void generateBullet()
    {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastShot > cooldown)
        {
            BulletType2 b(down);
            bullets.push_back(b);
            down = !down;
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
                    [](const BulletType2& b) { return !b.isActive(); }), bullets.end());

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
        if (myPlayer.isBlinking) return false;
        for (auto& bullet : bullets)
        {
            if (bullet.visible) if (checkCollision(bullet.bigBullet, myPlayer)) return true;
            if (!bullet.visible)
                for (auto& i:bullet.smallBullet)
                if (checkCollision(i, myPlayer)) return true;
        }
        return false;
    }
    bool endPhase()
    {
        return (phaseTime > PHASE_TIME);
    }
};

inline gamePhase doPhase2(Graphics& graphics, player& myPlayer)
{
    Sound collide(COLLIDE_SOUND_PATH);
    Phase2 phase2(graphics);
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
                phase2.updateTimePause(pauseDuration);
                myPlayer.updateTimePause(pauseDuration);
            }
        if (myPlayer.isDead())
        {
            SDL_Delay(500);
            return gamePhase::gameOver;
        }
        myPlayer.moveCheck();
        phase2.generateBullet();
        phase2.update();
        if (phase2.checkPhaseCollision(myPlayer))
        {
            if (graphics.sound) collide.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase2.render();
        graphics.presentScene();
        if (phase2.endPhase())
        {
            return gamePhase::Phase3;
        }
        SDL_Delay(10);
    }

    return gamePhase::quit;
}

#endif // _PHASE2__H
