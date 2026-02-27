#include "Paddle.h"

Paddle::Paddle(float x, float y, float w, float h, float speed)
    : x(x), y(y), width(w), height(h), speed(speed), score(0) {}

void Paddle::moveUp(float dt) {
    y -= speed * dt;
    if (y < 0.0f) y = 0.0f;
}

void Paddle::moveDown(float dt, float screenHeight) {
    y += speed * dt;
    if (y + height > screenHeight) y = screenHeight - height;
}

SDL_Rect Paddle::rect() const {
    return {
        static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(width), static_cast<int>(height)
    };
}
