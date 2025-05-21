#ifndef _PHASE_QUIT__H
#define _PHASE_QUIT__H

#include "music.h"
#include "graphics.h"
#include "texture.h"
#include "font.h"
#include "button.h"

#define MESSAGE_BOX_PATH "img\\rectangle.png"

#define FONT_PATH "assets\\PixelSansSerif.ttf"

const int text_size = 12;

    SDL_Rect quit_box = {150, 100, 500, 400};
    SDL_Rect question_box = {250, 150, 300, 20};
    SDL_Rect yes_box = {250, 200, 300, 50};
    SDL_Rect no_box = {250, 300, 300, 50};
    SDL_Rect music_box = {300, 400, 50, 50};
    SDL_Rect sound_box = {450, 400, 50, 50};

enum class quitRespond
{
    quit,
    play,
    none
};

class PhaseQuit
{
    Graphics& graphics;
    Texture quitQuestion;
    Button yes, no;
    Texture yesMessage, noMessage;
    Texture quitBox;
    Button music_on, music_off, sound_on, sound_off;


public:
    PhaseQuit(const PhaseQuit&) = delete;
    PhaseQuit& operator=(const PhaseQuit&) = delete;

    PhaseQuit(Graphics &_graphics) : graphics(_graphics),
    quitQuestion{_graphics.renderer},

    yes{_graphics, yes_box, MESSAGE_BOX_PATH},
    no{_graphics, no_box, MESSAGE_BOX_PATH},
    yesMessage{_graphics.renderer}, noMessage{_graphics.renderer},
    quitBox{_graphics.renderer},

    music_on(_graphics, music_box, MUSIC_ON),
    music_off(_graphics, music_box, MUSIC_OFF),
    sound_on(_graphics, sound_box, SOUND_ON),
    sound_off(_graphics, sound_box, SOUND_OFF)
    {
        Font gameFont;
        gameFont.loadFromFile(FONT_PATH, text_size);

        quitQuestion.loadFromRenderedText("Are you sure you want to quit?", gameFont, WHITE_COLOR);
        yesMessage.loadFromRenderedText("Yeah, let me go!", gameFont, WHITE_COLOR);
        noMessage.loadFromRenderedText("Nope, keep playing", gameFont, WHITE_COLOR);

        quitBox.loadFromFile(MESSAGE_BOX_PATH);
    }

    quitRespond processClick(SDL_Event &e)
    {
        if (yes.processClick(graphics, e)) return quitRespond::quit;
        if (no.processClick(graphics, e)) return quitRespond::play;

        //music and sound
        if (graphics.music)
        {
            if (music_on.processClick(graphics, e))
                graphics.music = false;
        }
        else
        {
            if (music_off.processClick(graphics, e))
                graphics.music = true;
        }

        if (graphics.sound)
        {
            if (sound_on.processClick(graphics, e))
                graphics.sound = false;
        }
        else
        {
            if (sound_off.processClick(graphics, e))
                graphics.sound = true;
        }

        return quitRespond::none;
    }

    void render()
    {
        quitBox.renderBasic(quit_box);
        quitQuestion.renderBasic(question_box);
        yes.renderWithText(yesMessage);
        no.renderWithText(noMessage);
        if (graphics.music)
        {
            music_on.render();
        }
        else music_off.render();

        if (graphics.sound)
        {
            sound_on.render();
        }
        else sound_off.render();
    }
};

quitRespond doPhaseQuit(Graphics &graphics)
{
    PhaseQuit phaseQuit(graphics);
    quitRespond res = quitRespond::none;
    SDL_Event e;
    while (res==quitRespond::none)
    {
        graphics.prepareScene();
        while (SDL_PollEvent(&e))
        {
            res = phaseQuit.processClick(e);
        }
        phaseQuit.render();
        graphics.presentScene();
        SDL_Delay(10);
    }
    return res;
}

#endif // _PHASE_QUIT__H
