#ifndef _INTRO__H
#define _INTRO__H

#include "defs.h"
#include "font.h"
#include "texture.h"
#include "music.h"

#define SOUND_ON "img\\sound_on.png"
#define SOUND_OFF "img\\sound_off.png"
#define MUSIC_ON "img\\music_on.png"
#define MUSIC_OFF "img\\music_off.png"
#define PLAY "img\\play.png"


const int title_size = 30;

bool clickIn(int x, int y, const SDL_Rect& rect)
{
    if (x > rect.x+ rect.w || x < rect.x) return false;
    if (y > rect.y+ rect.h || y < rect.y) return false;
    return true;
}

class Intro
{
    const Graphics& graphics; //cop tham bien -> class ko can destructor
    Texture title;
    Texture music_on, music_off, sound_on, sound_off;
    Texture play_button;
    Sound click;
    bool music;
    bool sound;

    public:
    Intro(const Intro&) = delete;
    Intro&operator = (const Intro&) = delete;
    Intro(const Graphics& _graphics) : graphics(_graphics),  title{_graphics.renderer},
    music_on{_graphics.renderer}, music_off{_graphics.renderer},
    sound_on{_graphics.renderer}, sound_off{_graphics.renderer},play_button{_graphics.renderer},
    click{CLICK_SOUND_PATH}
    {
        music = 1; sound = 1;
        Font gameFont;
        gameFont.loadFromFile(FONT_PATH, title_size);
        title.loadFromRenderedText("SHAPES ESCAPE", gameFont, WHITE_COLOR);

        music_on.loadFromFile(MUSIC_ON);
        music_off.loadFromFile(MUSIC_OFF);
        sound_on.loadFromFile(SOUND_ON);
        sound_off.loadFromFile(SOUND_OFF);
        play_button.loadFromFile(PLAY);
    }

    void render()
    {
        SDL_Rect play_b = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 100, 100};
        SDL_Rect music_b = {play_b.x-100, play_b.y+100, 100, 100};
        SDL_Rect sound_b = {play_b.x+100, play_b.y+100, 100, 100};
        graphics.prepareScene();

        title.render((SCREEN_WIDTH - title.getWidth())/2, 200);
        play_button.renderBasic(play_b);

        if (music)
        {
            music_on.renderBasic(music_b);
        }
        else music_off.renderBasic(music_b);

        if (sound)
        {
            sound_on.renderBasic(sound_b);
        }
        else sound_off.renderBasic(sound_b);
        graphics.presentScene();
    }

    bool processClickAndPlay(SDL_Event& e, Graphics& graphics)
    {
        SDL_Rect play_b = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 100, 100};
        SDL_Rect music_b = {play_b.x-100, play_b.y+100, 100, 100};
        SDL_Rect sound_b = {play_b.x+100, play_b.y+100, 100, 100};

        int x, y;
        SDL_GetMouseState(&x, &y);
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (graphics.sound) click.play();
            if (clickIn(x, y, play_b)) return true;
            if (clickIn(x, y, music_b))
            {
                music = !music;
                graphics.music = music;
            }
            if (clickIn(x, y, sound_b))
            {
                sound = !sound;
                graphics.sound = sound;
            }
        }

        return false;
    }//processClick

};

#endif // _INTRO__H
