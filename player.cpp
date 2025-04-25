#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>

#include "player.h"

void player::turnNorth()
{
        rect.y-=speed;
        rect.y = std::max(0, rect.y);
}

void player::turnSouth()
{
        rect.y+=speed;
        rect.y = std::min(rect.y, SCREEN_HEIGHT - rect.h);
}

void player::turnWest()
{
        rect.x-=speed;
        rect.x = std::max(rect.x, 0);
}

void player::turnEast()
{
        rect.x+=speed;
        rect.x = std:: min(rect.x, SCREEN_WIDTH - rect.w);
}

player::player():rect{SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_SIZE, PLAYER_SIZE}, speed(PLAYER_SPEED), lives(3)
{
    startCount = SDL_GetTicks();
    survivedTime = 0;
}

bool player::isDead()
{
    return (lives<=0);
}

void player::loseLife()
{
        isBlinking = true;
        blinkStartTime = SDL_GetTicks();
        lives--;
        if (lives == 2) rect = {rect.x, rect.y, PLAYER_SIZE/2, PLAYER_SIZE};
        if (lives == 1) rect = {rect.x, rect.y, PLAYER_SIZE/2, PLAYER_SIZE/2};
}

void player::render(const Graphics& graphics)
{

        if (visible)
        {
        Uint8 currentAlpha = 15;
        for (auto& trail: trails)
        {
            trail.render(currentAlpha, graphics);
            currentAlpha+=10;
        }
        graphics.setColor(PLAYER_COLOR);
        SDL_RenderFillRect(graphics.renderer, &rect);

        }
}

void player::moveCheck()
{
        blink();
        Uint32 current = SDL_GetTicks();
        survivedTime += current - startCount;
        startCount = current;

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) turnNorth();
        if (currentKeyStates[SDL_SCANCODE_DOWN]) turnSouth();
        if (currentKeyStates[SDL_SCANCODE_LEFT]) turnWest();
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) turnEast();
        trailDot currentTrail(this->rect);
        if (trails.size() > number_of_trail)
        {
            trails.pop_front();
        }
            trails.push_back(currentTrail);
}

void player::blink()
{
        if (!isBlinking) return;

        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeBlinked = currentTime - blinkStartTime;

        if (timeBlinked < BLINKING_TIME)
        {
            visible = (timeBlinked/200)%2 != 0;
        }
        else
        {
            isBlinking = false;
            visible = true;
        }
}

//trail
trailDot::trailDot()
{
        rect = {0, 0, 0, 0};
}

trailDot::trailDot(const SDL_Rect& Playerrect)
{
        rect = {Playerrect.x + Playerrect.w/4, Playerrect.y + Playerrect.h/4, Playerrect.w/2, Playerrect.h/2};
}

void trailDot::render(Uint8 alpha, const Graphics& graphics) const
    {
        SDL_SetRenderDrawColor(graphics.renderer, PLAYER_COLOR.r, PLAYER_COLOR.g, PLAYER_COLOR.b, alpha);
        SDL_RenderFillRect(graphics.renderer, &rect);
    }
