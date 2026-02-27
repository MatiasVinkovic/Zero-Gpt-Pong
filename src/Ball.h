#pragma once

struct Ball {
    float x, y;       // centre position (pixels)
    float vx, vy;     // velocity (pixels / second)
    float radius;     // collision / draw radius

    Ball();
    void  reset(float screenWidth, float screenHeight);
    void  update(float dt);
    float speed() const;
};
