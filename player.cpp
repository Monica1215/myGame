#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include <bits/stdc++.h>
#include "player.h"


player::player(Graphics &graphics): score(graphics.renderer), ripple(0, 0, 0, 0), ripple_t(graphics.renderer)
{
    rect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_SIZE, PLAYER_SIZE};
    speed = PLAYER_SPEED;
    lives = 3;
    isBlinking = false;
    blinkStartTime = 0;
    visible = true;
    trails.clear();
    survivedTime = 0;
    startCount = SDL_GetTicks();
    isDashing = false;
    lastVx = 0; lastVy = 0;
    lastDash = 0;

    lastScore = 0;
    gameFont.loadFromFile("assets\\Random Wednesday.ttf", 30);
    score.loadFromRenderedText(secondToTimer(lastScore), gameFont, WHITE_COLOR);

    ripple_t.loadFromFile(RIPPLE_PATH);
    ripple_t.setBlendMode(SDL_BLENDMODE_BLEND);

}

bool player::isDead()
{
    return (lives<=0);
}

void player::reset(Graphics &graphics)
{
    rect = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PLAYER_SIZE, PLAYER_SIZE};
    speed = PLAYER_SPEED;
    lives = 3;
    isBlinking = false;
    blinkStartTime = 0;
    visible = true;
    trails.clear();
    survivedTime = 0;
    startCount = SDL_GetTicks();
    isDashing = false;
    lastVx = 0; lastVy = 0;
    lastDash = 0;

    lastScore = 0;
    gameFont.loadFromFile("assets\\Random Wednesday.ttf", 30);
    score.loadFromRenderedText(secondToTimer(lastScore), gameFont, WHITE_COLOR);
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

        int survivedSeconds = survivedTime/1000;
        if (survivedSeconds!=lastScore)
        {
            lastScore = survivedSeconds;
            score.loadFromRenderedText(secondToTimer(lastScore), gameFont, WHITE_COLOR);
        }
        score.render(700, 30);
        if (isDashing) ripple.render(ripple_t);

}

void player::moveCheck()
{
        blink();
        Uint32 current = SDL_GetTicks();
        survivedTime += current - startCount;
        startCount = current;

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        int vx = 0;
        int vy = 0;
        if (currentKeyStates[SDL_SCANCODE_UP]) vy -= 1;
        if (currentKeyStates[SDL_SCANCODE_DOWN]) vy += 1;
        if (currentKeyStates[SDL_SCANCODE_LEFT]) vx -= 1;
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) vx += 1;

        if (!isDashing && currentKeyStates[SDL_SCANCODE_SPACE] && current - lastDash > DashCoolDown && survivedTime>FIRST_DASH)
        {
            Ripple tmp(rect.x + rect.w/2, rect.y + rect.h/2, 60, DASH_TIME);
            ripple = tmp;

            lastVx = vx; lastVy = vy;
            if (lastVx==0&&lastVy==0) lastVx = 1;
            if (lastVx!=0&&lastVy!=0)
            {
                float scale = 1.0f / sqrtf(2.0f);
                lastVx = lastVx * scale;
                lastVy = lastVy * scale;
            }
            isDashing = true;
            dashStart = current;
            lastDash = current;
        }
        if (isDashing)
        {
            ripple.update();
            rect.x += lastVx * DASH_SPEED;
            rect.y += lastVy * DASH_SPEED;
            if (current - dashStart >= DASH_TIME) {
                isDashing = false;
            }
        }
        else
        {
            rect.x += vx * PLAYER_SPEED;
            rect.y += vy * PLAYER_SPEED;
        }

        rect.x = std::clamp(rect.x, 0, SCREEN_WIDTH - rect.w);
        rect.y = std::clamp(rect.y, 0, SCREEN_HEIGHT - rect.h);

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
