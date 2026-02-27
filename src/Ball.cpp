#include "Ball.h"

#include <cmath>
#include <cstdlib>

static constexpr float BALL_INITIAL_SPEED = 300.0f;
static constexpr float BALL_RADIUS        = 8.0f;
static constexpr float PI                 = 3.14159265358979f;

Ball::Ball() : x(0.0f), y(0.0f), vx(0.0f), vy(0.0f), radius(BALL_RADIUS) {}

void Ball::reset(float screenWidth, float screenHeight) {
    x = screenWidth  / 2.0f;
    y = screenHeight / 2.0f;

    // Random angle -30..+30 degrees, random left/right direction
    float angle = (static_cast<float>(rand() % 60) - 30.0f) * PI / 180.0f;
    float dir   = (rand() % 2 == 0) ? 1.0f : -1.0f;

    vx = dir  * BALL_INITIAL_SPEED * cosf(angle);
    vy = BALL_INITIAL_SPEED        * sinf(angle);
}

void Ball::update(float dt) {
    x += vx * dt;
    y += vy * dt;
}

float Ball::speed() const {
    return sqrtf(vx * vx + vy * vy);
}
