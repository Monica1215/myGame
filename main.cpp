#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
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

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT) quit = true;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) myPlayer.myPlayer.turnNorth();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) myPlayer.myPlayer.turnSouth();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) myPlayer.myPlayer.turnWest();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) myPlayer.myPlayer.turnEast();

        myPlayer.render(graphics);
        graphics.presentScene();

        SDL_Delay(10);
    }

    myPlayer.quit();
    graphics.quit();
    return 0;
}
