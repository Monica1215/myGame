#ifndef _PHASE2__H
#define _PHASE2__H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "bullet.h"
#include "defs.h"
#include "bullet2.h"

class Phase2
{
    std::vector<BulletType2> bullets;
    Uint32 cooldown = 1000;
    Uint32 lastShot = 0;
    Uint32 startTime = 0;
    bool down = 1;

public:
    Phase2()
    {
        startTime = SDL_GetTicks();
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
        for (auto& bullet : bullets)
        {
            bullet.update();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                    [](const BulletType2& b) { return !b.isActive(); }), bullets.end());

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
        Uint32 currentTime = SDL_GetTicks();
        return (currentTime - startTime > PHASE_TIME);
    }
};

inline gamePhase doPhase2(const Graphics& graphics, player& myPlayer)
{
    SDL_Texture* bullet_texture = graphics.loadTexture(BULLET_FILE_PATH);
    Phase2 phase2;
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
            return gamePhase::gameOver;
        }
        myPlayer.blink();
        myPlayer.moveCheck();
        phase2.generateBullet();
        phase2.update();
        if (phase2.checkPhaseCollision(myPlayer)) myPlayer.loseLife();

        myPlayer.render(graphics);
        phase2.render(graphics, bullet_texture);
        graphics.presentScene();
        if (phase2.endPhase())
        {
            SDL_DestroyTexture(bullet_texture);
            return gamePhase::Phase3;
        }
        SDL_Delay(10);
    }

    SDL_DestroyTexture(bullet_texture);
    return gamePhase::quit;
}

#endif // _PHASE2__H
