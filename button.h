#ifndef _BUTTON__H_
#define _BUTTON__H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "music.h"
#include "graphics.h"

bool clickIn(int x, int y, const SDL_Rect& rect)
{
    if (x > rect.x+ rect.w || x < rect.x) return false;
    if (y > rect.y+ rect.h || y < rect.y) return false;
    return true;
}


struct Button
{
    Texture icon;
    Sound click;
    SDL_Rect rect;

    Button(const Graphics& graphics, SDL_Rect _rect, const std::string path): icon{graphics.renderer}, click{CLICK_SOUND_PATH}
    {
        rect = {_rect.x, _rect.y, _rect.w, _rect.h};
        icon.loadFromFile(path);
    }

    void render() const
    {
        icon.renderBasic(rect);
    }

    bool processClick(Graphics &graphics, SDL_Event &e)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (clickIn(x, y, rect)) icon.setColor(PLAYER_COLOR.r, PLAYER_COLOR.g, PLAYER_COLOR.b);
        else icon.setColor(255, 255, 255);

        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (graphics.sound) click.play();
            if (clickIn(x, y, rect)) return true;
        }

        return false;

    }
    void renderWithText(Texture& text)
    {
        Uint8 r, g, b;
        icon.getColor(r, g, b);
        text.setColor(r, g, b);
        SDL_Rect dsc = {rect.x + (rect.w-text.getWidth())/2,
                        rect.y + (rect.h-text.getHeight())/2,
                        text.getWidth(), text.getHeight()};

        icon.renderBasic(rect);
        text.renderBasic(dsc);
    }
};


#endif // _BUTTON__H_
