#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "bullet.h"
#include "intro.h"
using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

GameStates doIntro(Graphics& graphics)
{
    Intro intro(graphics);
    SDL_Event e;
    while (true)
    {
        intro.render();
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return GameStates::Quit;
            if (intro.processClickAndPlay(e)) return GameStates::Playing;

        }
        SDL_Delay(10);
    }
}


GameStates doPlaying(Graphics& graphics)
{

    player myPlayer;

    SDL_Texture* bullet_texture = graphics.loadTexture(BULLET_FILE_PATH);

    SDL_Event e;
    bool quit = false;

    vector<Bullet> bullets;
    Uint32 cooldown = 500;
    Uint32 lastShot = 0;
        while (!quit)
    {
        Uint32 currentTime = SDL_GetTicks();
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) quit = true;
        if (myPlayer.isDead())
        {
            cout << "GAME OVER";
            quit = true;
            break;
        }
        myPlayer.blink();
        myPlayer.moveCheck();

        if (currentTime - lastShot > cooldown)
        {
            int y = (int)(generateRandom()*SCREEN_WIDTH);

            bullets.push_back({0, y, 20,  INITIAL_BULLET_SPEED});
            lastShot = currentTime;
        }

        for (auto& bullet: bullets)
        {
            bullet.update();
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                    [](const Bullet& b) { return !b.isActive(); }), bullets.end());

        for (auto& bullet : bullets)
        {
            bullet.render(bullet_texture, graphics);
        }
        for (auto& bullet : bullets)
        {
            if (!myPlayer.isBlinking &&checkCollision(bullet, myPlayer))
                myPlayer.loseLife();
        }

        myPlayer.render(graphics);
        graphics.presentScene();

        SDL_Delay(16);
    }
    SDL_DestroyTexture(bullet_texture);
    return GameStates::Quit;
}


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    GameStates current_states = GameStates::Intro;
    while (current_states!=GameStates::Quit)
    {
        switch (current_states)
        {
        case GameStates::Intro:
            current_states = doIntro(graphics);
            break;
        case GameStates::Playing:
            current_states = doPlaying(graphics);
            break;
        default: return -1;
        }
    }


    return 0;
}
