#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "player.h"
#include "bullet.h"
#include "intro.h"
#include "phase1.h"
#include "phase2.h"
#include "phase3.h"
#include "phase4.h"
#include "music.h"
#include "phaseTransition.h"
#include "phaseQuit.h"
#include "button.h"
#include "gameOver.h"
using namespace std;

GameStates doIntro(Graphics& graphics)
{
    Intro intro(graphics);
    SDL_Event e;
    if (graphics.music)
        graphics.mus.play();
    while (true)
    {
        graphics.prepareScene();
        intro.render();
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return GameStates::Quit;
            if (intro.processClickAndPlay(e)) return GameStates::Playing;

        }
        graphics.presentScene();
        SDL_Delay(10);
    }
}


GameStates doPlaying(Graphics& graphics, player &myPlayer)
{
    int gameLoop = 0;
    gamePhase currentPhase = gamePhase::Phase1;
    while (currentPhase!=gamePhase::quit)
    switch (currentPhase)
    {
        case gamePhase::Phase1:
            currentPhase = doPhase1(graphics, myPlayer, gameLoop);
            currentPhase = doPhaseTransition(graphics, myPlayer, currentPhase);
            break;
        case gamePhase::Phase2:
            currentPhase = doPhase2(graphics, myPlayer, gameLoop);
            currentPhase = doPhaseTransition(graphics, myPlayer, currentPhase);
        case gamePhase::Phase3:
            currentPhase = doPhase3(graphics, myPlayer, gameLoop);
            currentPhase = doPhaseTransition(graphics, myPlayer, currentPhase);
            break;
        case gamePhase::Phase4:
            currentPhase = doPhase4(graphics, myPlayer, gameLoop);
            currentPhase = doPhaseTransition(graphics, myPlayer, currentPhase);
            gameLoop++;
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

GameStates doGameOver(Graphics &graphics, player &myPlayer)
{
    GameOver gameOver(graphics, myPlayer);
    gameOverRespond res = gameOverRespond::none;
    SDL_Event e;
    while (true)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
        {
            res = gameOver.processClick(e);
            switch(res)
            {
            case gameOverRespond::home:
                return GameStates::Intro;
            case gameOverRespond::replay:
                return GameStates::Playing;
            case gameOverRespond::quit:
                return GameStates::Quit;
            default:
                break;
            }
        }
        gameOver.render();
        graphics.presentScene();
    }
}


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    player myPlayer(graphics);
    Uint32 start;
    GameStates current_states = GameStates::Intro;
    while (current_states!=GameStates::Quit)
    {
        switch (current_states)
        {
        case GameStates::Intro:
            start = SDL_GetTicks();
            current_states = doIntro(graphics);
            myPlayer.updateTimePause(SDL_GetTicks() - start);
            break;
        case GameStates::Playing:
            cout << boolalpha << myPlayer.isDead();
            current_states = doPlaying(graphics, myPlayer);
            break;
        case GameStates::GameOver:
            current_states = doGameOver(graphics, myPlayer);
            cout << static_cast<int>(current_states);
            cout << "GAME OVER";
            myPlayer.reset(graphics);
            break;
        case GameStates::Quit:
            cout << "QUIT";
            break;
        default :
            return -1;
        }
    }

    //doPhaseQuit(graphics);
    return 0;
}
