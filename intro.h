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

const int title_size = 50;

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
    Uint32 startTime;

        SDL_Rect play_b = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 100, 100};
        SDL_Rect music_b = {play_b.x-200, play_b.y+100, 100, 100};
        SDL_Rect sound_b = {play_b.x+200, play_b.y+100, 100, 100};
        SDL_Rect title_b = {(SCREEN_WIDTH - title.getWidth())/2, 100, title.getWidth(), title.getHeight()};
        SDL_Rect line_dsc = {0, play_b.y - 50, SCREEN_WIDTH, 200};
        SDL_Rect line_src = {0, 0, 0, 0};
    SDL_Texture* line;
    int line_time = 4000;
    public:
    Intro(const Intro&) = delete;
    Intro&operator = (const Intro&) = delete;
    Intro(const Graphics& _graphics) : graphics(_graphics),  title{_graphics.renderer},
    music_on{_graphics.renderer}, music_off{_graphics.renderer},
    sound_on{_graphics.renderer}, sound_off{_graphics.renderer},play_button{_graphics.renderer},
    click{CLICK_SOUND_PATH}
    {
        line = graphics.loadTexture(LINE_PATH);
        SDL_QueryTexture(line, NULL, NULL, &line_src.w, &line_src.h);
        startTime = SDL_GetTicks();

        music = 1; sound = 1;
        Font gameFont;
        gameFont.loadFromFile(FONT_PATH, title_size);
        title.loadFromRenderedText("SHAPES ESCAPE", gameFont, ENEMY_COLOR);

        music_on.loadFromFile(MUSIC_ON);
        music_off.loadFromFile(MUSIC_OFF);
        sound_on.loadFromFile(SOUND_ON);
        sound_off.loadFromFile(SOUND_OFF);
        play_button.loadFromFile(PLAY);
    }

    void render()
    {
        Uint32 current = SDL_GetTicks();
        float lineRatio = 1.0*((current - startTime)%line_time)/line_time;
        SDL_Rect src = {0, 0, static_cast<int>(lineRatio*line_src.w), line_src.h};
        SDL_Rect dsc = {line_dsc.x, line_dsc.y, static_cast<int>(lineRatio*line_dsc.w), line_dsc.h};
        SDL_RenderCopy(graphics.renderer, line, &src, &dsc);

        title.render((SCREEN_WIDTH - title.getWidth())/2, 100);
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

    }

    bool processClickAndPlay(SDL_Event& e, Graphics& graphics)
    {
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

    ~Intro()
    {
        SDL_DestroyTexture(line);
        line = NULL;
    }
};



#endif // _INTRO__H
