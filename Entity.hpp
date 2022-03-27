#include <SDL.h>

class Entity
{
public:
    SDL_Texture* tex;
    SDL_Surface* surf;
    SDL_Rect srcRect;
    SDL_Rect rect;
    float x;
    float y;
    float w;
    float h;
    float speed;
    float speedX;
    float speedY;
    float actVar;
    float scale;
    float alpha;
    float randAlpha;
    float schX;
    float schY;
    float randAct;
    float currentFrame;


    Entity(SDL_Texture* etex, int srcW, int srcH, float ex, float ey, int ew, int eh, float espeed, bool centered);

    Entity() = default;

    void draw(SDL_Renderer* renderer);

    void update();

    void moveUpdate(double deltaTime, float damp);

    void downAlpha(int speed);
    void upAlpha(int speed);
};