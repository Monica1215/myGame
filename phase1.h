#ifndef _PHASE1__H
#define _PHASE1__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
class Phase1
{
    std::vector<Bullet> bullets;
    Uint32 cooldown = 500;
    Uint32 lastShot = 0;
    Uint32 startTime = 0;

public:
    Phase1()
    {
        startTime = SDL_GetTicks();
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
        for (auto& bullet : bullets)
        {
            bullet.update();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return !b.isActive(); }), bullets.end());

    }
    void render(const Graphics &graphics, SDL_Texture* bullet_texture) const
    {
        for (auto& bullet : bullets)
        {
            bullet.render(bullet_texture, graphics);
        }
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
        Uint32 currentTime = SDL_GetTicks();
        return (currentTime - startTime > PHASE_TIME);
    }
};

inline gamePhase doPhase1(const Graphics& graphics, player& myPlayer)
{
    SDL_Texture* bullet_texture = graphics.loadTexture(BULLET_FILE_PATH);
    Phase1 phase1;
    SDL_Event e;
    Sound collide(COLLIDE_SOUND_PATH);
    bool quit = false;
    while (!quit)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) quit = true;
        if (myPlayer.isDead())
        {
            SDL_Delay(500);
            SDL_DestroyTexture(bullet_texture);
            return gamePhase::gameOver;
        }
        myPlayer.blink();
        myPlayer.moveCheck();
        phase1.generateBullet();
        phase1.update();
        if (phase1.checkPhaseCollision(myPlayer))
        {
            if (graphics.sound) collide.play();
            myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        phase1.render(graphics, bullet_texture);
        graphics.presentScene();
        if (phase1.endPhase())
        {
            SDL_DestroyTexture(bullet_texture);
            return gamePhase::Phase2;
        }
        SDL_Delay(10);
    }

    SDL_DestroyTexture(bullet_texture);
    return gamePhase::quit;
}

#endif // _PHASE1__
