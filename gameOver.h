#ifndef _GAMEOVER__H
#define _GAMEOVER__H

#include <bits/stdc++.h>
#include "font.h"
#include "defs.h"
#include "texture.h"
#include "button.h"
    SDL_Rect home_b = {200, 400, 80, 80};
    SDL_Rect replay_b = {520, 400, 80, 80};

    SDL_Rect gameover_b = {150, 100, SCREEN_WIDTH-300, 150};

enum class gameOverRespond
{
    quit,
    home,
    replay,
    none
};


class GameOver
{
    Graphics &graphics;
    Texture title;
    Texture survived;
    Texture bestPlay;

    Button home;
    Button replay;

    public:
    GameOver(Graphics& _graphics, const player &myPlayer): graphics(_graphics),
    title(_graphics.renderer), survived(_graphics.renderer), bestPlay(_graphics.renderer),
    home(_graphics, home_b, "img\\home.png"), replay(_graphics, replay_b, "img\\replay.png")
    {
        Font gameFont;
        gameFont.loadFromFile("assets\\Random Wednesday.ttf", 30);

        Font gameFont2;
        gameFont2.loadFromFile("assets\\Random Wednesday.ttf", 120);

        Uint32 highScore;
        std::ifstream inFile("highscore.txt");
        if (inFile.is_open()) {
        inFile >> highScore;
        }
        inFile.close();

        if (myPlayer.survivedTime > highScore)
        {
            highScore = myPlayer.survivedTime;
            std::ofstream outFile("highscore.txt");
            if (outFile.is_open())
            {
                outFile << highScore;
                outFile.close();
            }
        }

        int seconds = myPlayer.survivedTime/1000;
        int highScoreSeconds = highScore/1000;

        title.loadFromRenderedText("Game Over!", gameFont2, WHITE_COLOR);
        survived.loadFromRenderedText("Time survived: "+secondToTimer(seconds), gameFont, WHITE_COLOR);
        bestPlay.loadFromRenderedText("Best play: "+secondToTimer(highScoreSeconds), gameFont, WHITE_COLOR);

    }

    gameOverRespond processClick(SDL_Event &e)
    {
        if (home.processClick(graphics, e)) return gameOverRespond::home;
        if (replay.processClick(graphics, e)) return gameOverRespond::replay;
        if (e.type == SDL_QUIT) return gameOverRespond::quit;
        return gameOverRespond::none;
    }

    void render()
    {
        title.renderBasic(gameover_b);
        survived.render(300, 300);
        bestPlay.render(300, 350);

        home.render();
        replay.render();
    }

};


#endif // _GAMEOVER__H
