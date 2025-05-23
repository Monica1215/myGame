#ifndef _DEFS__H
#define _DEFS__H


#include <SDL.h>
#include <bits/stdc++.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WINDOW_TITLE "SHAPE ESCAPE"
const SDL_Color PLAYER_COLOR = {27, 239, 255, 255};
const SDL_Color BACKGROUND_COLOR = {0, 19, 24};
const SDL_Color WHITE_COLOR = {255, 255, 255, 0};
const SDL_Color ENEMY_COLOR = {255, 0 ,102, 255};

const std::string MUSIC_THEME_PATH = "assets\\popstar_love.mp3";
#define PHASE_TIME 10000
#define BULLET_FILE_PATH "img\\bullet.png"

#define CLICK_SOUND_PATH "assets\\click.wav"
#define COLLIDE_SOUND_PATH "assets\\collide.wav"
#define LAZER_SOUND_PATH "assets\\lazer-shot.wav"

#define BACKGROUND_PATH "img\\background.png"

inline float generateRandom()
{
    return (float) rand() / RAND_MAX;
}

inline std::string secondToTimer(int totalSeconds)
{
    int seconds = totalSeconds%60;
    int minutes = (totalSeconds/60)%60;
    int hours = totalSeconds/3600;

    std::ostringstream oss;
    if (hours>0) oss << std::setw(2) << std::setfill('0') << hours << ":";
    oss    << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

enum class GameStates
{
    Intro,
    Playing,
    Quit,
    GameOver,
    Null
};

enum class gamePhase
{
    Phase1,
    Phase2,
    Phase3,
    Phase4,
    quit,
    gameOver
};

#endif // _DEFS__H
