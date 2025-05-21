#ifndef _INTRO__H
#define _INTRO__H

#include "defs.h"
#include "font.h"
#include "texture.h"
#include "music.h"
#include "button.h"

#define SOUND_ON "img\\sound_on.png"
#define SOUND_OFF "img\\sound_off.png"
#define MUSIC_ON "img\\music_on.png"
#define MUSIC_OFF "img\\music_off.png"
#define PLAY "img\\play.png"
#define LINE_PATH "img\\line.png"

const int title_size = 160;
        SDL_Rect play_b = {SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2, 100, 100};
        SDL_Rect music_b = {play_b.x-200, play_b.y+100, 100, 100};
        SDL_Rect sound_b = {play_b.x+200, play_b.y+100, 100, 100};
        SDL_Rect title_b = {100, 100, SCREEN_WIDTH-200, 150};

class Intro
{
    Graphics& graphics;
    Texture background;
    Texture title;
    Button play, music_on, music_off, sound_on, sound_off;
    Uint32 startTime;

        SDL_Rect line_dsc = {0, play_b.y - 50, SCREEN_WIDTH, 200};
        SDL_Rect line_src = {0, 0, 0, 0};

    SDL_Texture* line;
    int line_time = 4000;
    public:
    Intro(const Intro&) = delete;
    Intro&operator = (const Intro&) = delete;
    Intro(Graphics& _graphics) : graphics(_graphics),background{_graphics.renderer}, title{_graphics.renderer},
        play(_graphics, play_b, PLAY),
        music_on(_graphics, music_b, MUSIC_ON),
        music_off(_graphics, music_b, MUSIC_OFF),
        sound_on(_graphics, sound_b, SOUND_ON),
        sound_off(_graphics, sound_b, SOUND_OFF)
    {
        line = graphics.loadTexture(LINE_PATH);
        SDL_QueryTexture(line, NULL, NULL, &line_src.w, &line_src.h);
        startTime = SDL_GetTicks();

        Font gameFont;
        gameFont.loadFromFile("assets\\Random Wednesday.ttf", title_size);
        background.loadFromFile("img\\background.png");
        title.loadFromRenderedText("Shapes Escape", gameFont, ENEMY_COLOR);

    }

    void render()
    {
        background.renderBasic({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
        Uint32 current = SDL_GetTicks();
        float lineRatio = 1.0*((current - startTime)%line_time)/line_time;
        SDL_Rect src = {0, 0, static_cast<int>(lineRatio*line_src.w), line_src.h};
        SDL_Rect dsc = {line_dsc.x, line_dsc.y, static_cast<int>(lineRatio*line_dsc.w), line_dsc.h};
        SDL_RenderCopy(graphics.renderer, line, &src, &dsc);

        title.renderBasic(title_b);
        play.render();
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

bool processClickAndPlay(SDL_Event& e)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (play.processClick(graphics, e)) return true;

        //music and sound
        if (graphics.music)
        {
            if (music_on.processClick(graphics, e))
                {
                    graphics.music = false;
                    graphics.mus.pause();
                }
        }
        else
        {
            if (music_off.processClick(graphics, e))
                {
                    graphics.music = true;
                    graphics.mus.play();
                }
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

        return false;
    }//processClick

    ~Intro()
    {
        SDL_DestroyTexture(line);
        line = NULL;
    }
};



#endif // _INTRO__H
