#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "bullet.h"
#include "intro.h"
#include "phase1.h"
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


GameStates doPlaying(const Graphics& graphics)
{
    player myPlayer;
    gamePhase currentPhase = gamePhase::Phase1;
    while (currentPhase!=gamePhase::quit)
    switch (currentPhase)
    {
        case gamePhase::Phase1:
            currentPhase = doPhase1(graphics, myPlayer);
            break;
        case gamePhase::gameOver:
            return GameStates::GameOver;
            break;
        case gamePhase::quit:
            return GameStates::Quit;
        default:
            return GameStates::Quit;
    }
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
        case GameStates::GameOver:
            current_states = GameStates::Quit;
            cout << "GAME OVER";
            break;
        case GameStates::Quit:
            cout << "QUIT";
            break;
        default :
            return -1;
        }
    }


    return 0;
}
