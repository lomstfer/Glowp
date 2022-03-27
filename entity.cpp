#include "Entity.hpp"
#include "Ftint.hpp"
#include <stdlib.h>
#include <math.h>


Entity::Entity(SDL_Texture* etex, int srcW, int srcH, float ex, float ey, int ew, int eh, float espeed, bool centered)
{
    tex = etex;

    x = ex;
    y = ey;
    w = ew;
    h = eh;
    if (centered)
    {
        x -= w / 2;
        y -= h / 2;
    }
    
    
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = srcW;
    srcRect.h = srcH;

    rect.x = ftint(x);
    rect.y = ftint(y);
    rect.w = ftint(w);
    rect.h = ftint(h);

    speed = espeed;
    speedX = 0;
    speedY = 0;
    actVar = 0;
    scale = 1;
    alpha = 0;
    randAlpha = rand() % 200 + 56;
    actVar = 0;
    randAct = rand() % 200 + 56;
    
    currentFrame = 0;
}

void Entity::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, tex, &srcRect, &rect);
}

void Entity::update()
{
    rect.w = ftint(w * scale);
    rect.h = ftint(h * scale);
}

void Entity::moveUpdate(double deltaTime, float damp)
{
    // slow down x speed
	speedX *= pow(damp, deltaTime);
	if (fabsf(speedX) < 0.001)
	{
		speedX = 0;
	}

	// slow down y speed
	speedY *= pow(damp, deltaTime);
	if (fabsf(speedY) < 0.001)
	{
		speedY = 0;
	}
    x += speedX * speed * deltaTime;
    y += speedY * speed * deltaTime;
    rect.x = ftint(x);
    rect.y = ftint(y);
}

void Entity::downAlpha(int speed)
{
    if (alpha > 0)
        alpha -= speed;
    if (alpha < 0)
        alpha = 0;
    
    SDL_SetTextureAlphaMod(tex, alpha);
}

void Entity::upAlpha(int speed)
{
    if (alpha < randAlpha)
        alpha += speed;
    if (alpha >= randAlpha)
        alpha = randAlpha;

    SDL_SetTextureAlphaMod(tex, alpha);
}