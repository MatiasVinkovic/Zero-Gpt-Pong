#pragma once

#include <SDL2/SDL.h>

struct Paddle {
    float x, y;
    float width, height;
    float speed;
    int   score;

    Paddle(float x, float y, float w, float h, float speed);
    void     moveUp  (float dt);
    void     moveDown(float dt, float screenHeight);
    SDL_Rect rect() const;
};
