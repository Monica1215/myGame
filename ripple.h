#ifndef _RIPPLE__H
#define _RIPPLE__H

#include "texture.h"
#define RIPPLE_PATH "img\\ripple.png"

class Ripple
{
    int x, y;
    int radius, maxRadius;
    Uint8 alpha;
    Uint32 startTime = 0;
    Uint32 duration;

public:
    Ripple(int _x, int _y, int r, Uint32 d)
    {
        x = _x;
        y = _y;
        radius = 0;
        duration = d;
        alpha = 255;
        startTime = SDL_GetTicks();
        maxRadius = r;
    }
    bool isAlive() const
    {
        return (SDL_GetTicks()-startTime <= duration);
    }

    void update()
    {
        if (!isAlive()) return;
        Uint32 elapsed = SDL_GetTicks() - startTime;
        float t = 1.0*elapsed/duration;
        radius = maxRadius*t;
        alpha = 255*(1-t);
    }
    void render(Texture &texture) const
    {
        if (!isAlive()) return;
        texture.setAlpha(alpha);
        SDL_Rect dsc = {x-radius, y-radius, 2*radius, 2*radius};
        texture.renderBasic(dsc);
    }
};

#endif // _RIPPLE__H
