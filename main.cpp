#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "bullet.h"
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


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    player myPlayer;
    myPlayer.init_and_loadTexture(PLAYER_FILE_PATH, graphics);

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
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) myPlayer.myPlayer.turnNorth();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) myPlayer.myPlayer.turnSouth();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) myPlayer.myPlayer.turnWest();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) myPlayer.myPlayer.turnEast();

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

        myPlayer.render(graphics);
        graphics.presentScene();

        SDL_Delay(16);
    }

    myPlayer.quit();
    graphics.quit();
    return 0;
}
