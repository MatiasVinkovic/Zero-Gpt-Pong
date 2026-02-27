#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Ball.h"
#include "Paddle.h"

class Game {
public:
    static constexpr int   SCREEN_WIDTH   = 800;
    static constexpr int   SCREEN_HEIGHT  = 600;
    static constexpr int   WINNING_SCORE  = 7;

    Game();
    ~Game();

    bool init();
    void run();
    void cleanup();

private:
    // Game-loop stages
    void handleEvents();
    void update(float dt);
    void render();

    // Physics
    void resolveBallWallCollision();
    void resolveBallPaddleCollision(Paddle& paddle, bool isLeft);

    // Rendering helpers
    void drawRect    (int x, int y, int w, int h);
    void drawCircle  (int cx, int cy, int r);
    void drawDigit   (int digit, int x, int y, int segLen, int segThick);
    void drawScore   (int score, int cx, int cy);
    void drawDashedLine();
    void drawGameOver();

    // Procedural audio
    Mix_Chunk* generateBeep(int frequencyHz, int durationMs, float volume = 1.0f);

    // SDL objects
    SDL_Window*   window;
    SDL_Renderer* renderer;

    // Game objects
    Ball   ball;
    Paddle leftPaddle;
    Paddle rightPaddle;

    // Sounds
    Mix_Chunk* soundPaddleHit;
    Mix_Chunk* soundWallHit;
    Mix_Chunk* soundScore;
    Mix_Chunk* soundWin;

    // State
    bool          running;
    bool          gameOver;
    bool          winSoundPlayed;
    int           winner;      // 1 = left player, 2 = right player
    const Uint8*  keys;
};
