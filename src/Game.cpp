#include "Game.h"

#include <cmath>
#include <cstdlib>

// Layout
static constexpr int   PADDLE_WIDTH         = 12;
static constexpr int   PADDLE_HEIGHT        = 80;
static constexpr int   PADDLE_MARGIN        = 20;
static constexpr float PADDLE_SPEED         = 420.0f;
static constexpr float BALL_MAX_SPEED       = 700.0f;
static constexpr float BALL_SPEED_INCREMENT = 25.0f;
static constexpr float PI                   = 3.14159265358979f;
static constexpr int   SERVE_DELAY_MS       = 500; // pause between a score and the next serve

// 7-segment display bitmap (segments: top, top-left, top-right, middle,
//                                     bottom-left, bottom-right, bottom)
static constexpr bool DIGIT_SEGS[10][7] = {
    { true,  true,  true,  false, true,  true,  true  }, // 0
    { false, false, true,  false, false, true,  false }, // 1
    { true,  false, true,  true,  true,  false, true  }, // 2
    { true,  false, true,  true,  false, true,  true  }, // 3
    { false, true,  true,  true,  false, true,  false }, // 4
    { true,  true,  false, true,  false, true,  true  }, // 5
    { true,  true,  false, true,  true,  true,  true  }, // 6
    { true,  false, true,  false, false, true,  false }, // 7
    { true,  true,  true,  true,  true,  true,  true  }, // 8
    { true,  true,  true,  true,  false, true,  true  }, // 9
};

// ─────────────────────────────────────────────────────────────────────────────
// Construction / initialisation
// ─────────────────────────────────────────────────────────────────────────────

Game::Game()
    : window(nullptr), renderer(nullptr),
      leftPaddle(
          PADDLE_MARGIN,
          Game::SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f,
          PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED),
      rightPaddle(
          Game::SCREEN_WIDTH - PADDLE_MARGIN - PADDLE_WIDTH,
          Game::SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f,
          PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED),
      soundPaddleHit(nullptr), soundWallHit(nullptr),
      soundScore(nullptr),     soundWin(nullptr),
      running(false), gameOver(false), winSoundPlayed(false),
      winner(0), keys(nullptr)
{}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) {
        SDL_Log("Mix_OpenAudio failed (audio disabled): %s", Mix_GetError());
        // Non-fatal: game runs silently without audio
    }

    window = SDL_CreateWindow(
        "Zero-GPT-Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        SDL_Quit();
        return false;
    }

    // Generate procedural sounds
    soundPaddleHit = generateBeep(480, 60);
    soundWallHit   = generateBeep(320, 40);
    soundScore     = generateBeep(220, 300);
    soundWin       = generateBeep(600, 500, 0.8f);

    keys = SDL_GetKeyboardState(nullptr);
    ball.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
    running = true;
    return true;
}

void Game::run() {
    Uint32      lastTime = SDL_GetTicks();
    const float MAX_DT   = 1.0f / 30.0f;   // cap physics step

    SDL_Delay(SERVE_DELAY_MS); // brief pause before the serve

    while (running) {
        Uint32 now = SDL_GetTicks();
        float  dt  = (now - lastTime) / 1000.0f;
        lastTime   = now;
        if (dt > MAX_DT) dt = MAX_DT;

        handleEvents();
        update(dt);
        render();
    }
}

void Game::cleanup() {
    auto freeChunk = [](Mix_Chunk*& c) {
        if (c) { free(c->abuf); delete c; c = nullptr; }
    };
    freeChunk(soundPaddleHit);
    freeChunk(soundWallHit);
    freeChunk(soundScore);
    freeChunk(soundWin);

    if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
    if (window)   { SDL_DestroyWindow(window);     window   = nullptr; }
    Mix_CloseAudio();
    SDL_Quit();
}

// ─────────────────────────────────────────────────────────────────────────────
// Game-loop stages
// ─────────────────────────────────────────────────────────────────────────────

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                running = false;
                break;
            case SDLK_r:
                if (gameOver) {
                    // Restart
                    leftPaddle.score  = 0;
                    rightPaddle.score = 0;
                    leftPaddle.y  = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
                    rightPaddle.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
                    ball.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
                    gameOver       = false;
                    winner         = 0;
                    winSoundPlayed = false;
                    SDL_Delay(SERVE_DELAY_MS);
                }
                break;
            default:
                break;
            }
        }
    }
}

void Game::update(float dt) {
    if (gameOver) {
        if (!winSoundPlayed && soundWin) {
            Mix_PlayChannel(-1, soundWin, 0);
            winSoundPlayed = true;
        }
        return;
    }

    // Paddle input — left: W/S, right: Up/Down
    if (keys[SDL_SCANCODE_W])    leftPaddle.moveUp(dt);
    if (keys[SDL_SCANCODE_S])    leftPaddle.moveDown(dt, SCREEN_HEIGHT);
    if (keys[SDL_SCANCODE_UP])   rightPaddle.moveUp(dt);
    if (keys[SDL_SCANCODE_DOWN]) rightPaddle.moveDown(dt, SCREEN_HEIGHT);

    ball.update(dt);

    resolveBallWallCollision();
    resolveBallPaddleCollision(leftPaddle,  true);
    resolveBallPaddleCollision(rightPaddle, false);

    // Scoring
    if (ball.x + ball.radius < 0.0f) {
        rightPaddle.score++;
        if (soundScore) Mix_PlayChannel(-1, soundScore, 0);
        if (rightPaddle.score >= WINNING_SCORE) {
            gameOver = true; winner = 2;
        } else {
            ball.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_Delay(SERVE_DELAY_MS);
        }
    } else if (ball.x - ball.radius > SCREEN_WIDTH) {
        leftPaddle.score++;
        if (soundScore) Mix_PlayChannel(-1, soundScore, 0);
        if (leftPaddle.score >= WINNING_SCORE) {
            gameOver = true; winner = 1;
        } else {
            ball.reset(SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_Delay(SERVE_DELAY_MS);
        }
    }
}

void Game::render() {
    // Background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    drawDashedLine();

    // Paddles
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect lr = leftPaddle.rect();
    SDL_RenderFillRect(renderer, &lr);
    SDL_Rect rr = rightPaddle.rect();
    SDL_RenderFillRect(renderer, &rr);

    // Ball
    drawCircle(static_cast<int>(ball.x), static_cast<int>(ball.y),
               static_cast<int>(ball.radius));

    // Score display
    drawScore(leftPaddle.score,  SCREEN_WIDTH / 4,       30);
    drawScore(rightPaddle.score, 3 * SCREEN_WIDTH / 4,   30);

    if (gameOver) drawGameOver();

    SDL_RenderPresent(renderer);
}

// ─────────────────────────────────────────────────────────────────────────────
// Physics
// ─────────────────────────────────────────────────────────────────────────────

void Game::resolveBallWallCollision() {
    if (ball.y - ball.radius <= 0.0f) {
        ball.y  = ball.radius;
        ball.vy = fabsf(ball.vy);
        if (soundWallHit) Mix_PlayChannel(-1, soundWallHit, 0);
    }
    if (ball.y + ball.radius >= SCREEN_HEIGHT) {
        ball.y  = SCREEN_HEIGHT - ball.radius;
        ball.vy = -fabsf(ball.vy);
        if (soundWallHit) Mix_PlayChannel(-1, soundWallHit, 0);
    }
}

void Game::resolveBallPaddleCollision(Paddle& paddle, bool isLeft) {
    // Reject if ball is moving away from this paddle
    if ( isLeft && ball.vx >= 0.0f) return;
    if (!isLeft && ball.vx <= 0.0f) return;

    SDL_Rect r = paddle.rect();

    // Closest point on the paddle rect to the ball centre (AABB–circle test)
    float closestX = fmaxf(static_cast<float>(r.x),
                    fminf(ball.x, static_cast<float>(r.x + r.w)));
    float closestY = fmaxf(static_cast<float>(r.y),
                    fminf(ball.y, static_cast<float>(r.y + r.h)));

    float dx    = ball.x - closestX;
    float dy    = ball.y - closestY;
    float dist2 = dx * dx + dy * dy;

    if (dist2 > ball.radius * ball.radius) return;

    // Compute outgoing angle based on relative hit position (-1..+1 on paddle)
    float centre   = static_cast<float>(r.y) + r.h / 2.0f;
    float relative = (ball.y - centre) / (r.h / 2.0f);
    float angle    = relative * (PI / 3.0f); // max ±60°

    float newSpeed = fminf(ball.speed() + BALL_SPEED_INCREMENT, BALL_MAX_SPEED);
    ball.vx = (isLeft ? 1.0f : -1.0f) * newSpeed * cosf(angle);
    ball.vy = newSpeed * sinf(angle);

    // Push ball clear of the paddle face
    if (isLeft)
        ball.x = static_cast<float>(r.x + r.w) + ball.radius;
    else
        ball.x = static_cast<float>(r.x)        - ball.radius;

    if (soundPaddleHit) Mix_PlayChannel(-1, soundPaddleHit, 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Rendering helpers
// ─────────────────────────────────────────────────────────────────────────────

void Game::drawRect(int x, int y, int w, int h) {
    SDL_Rect r = { x, y, w, h };
    SDL_RenderFillRect(renderer, &r);
}

// Filled circle via horizontal scanlines (midpoint / Bresenham)
void Game::drawCircle(int cx, int cy, int r) {
    for (int dy = -r; dy <= r; dy++) {
        int dx = static_cast<int>(sqrtf(static_cast<float>(r * r - dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

// 7-segment digit at position (x, y) with given segment length and thickness
void Game::drawDigit(int digit, int x, int y, int segLen, int segThick) {
    if (digit < 0 || digit > 9) return;
    const bool* s   = DIGIT_SEGS[digit];
    int         mid = y + segLen - segThick / 2;

    if (s[0]) drawRect(x,                   y,   segLen, segThick); // top
    if (s[1]) drawRect(x,                   y,   segThick, segLen); // top-left
    if (s[2]) drawRect(x + segLen - segThick, y,  segThick, segLen); // top-right
    if (s[3]) drawRect(x,                   mid, segLen, segThick); // middle
    if (s[4]) drawRect(x,                   mid, segThick, segLen); // bottom-left
    if (s[5]) drawRect(x + segLen - segThick, mid, segThick, segLen); // bottom-right
    if (s[6]) drawRect(x, y + 2 * segLen - segThick, segLen, segThick); // bottom
}

void Game::drawScore(int score, int cx, int cy) {
    const int SEG_LEN   = 30;
    const int SEG_THICK = 5;
    const int DIGIT_W   = SEG_LEN + 8;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if (score >= 10) {
        int startX = cx - DIGIT_W;
        drawDigit(score / 10, startX,              cy, SEG_LEN, SEG_THICK);
        drawDigit(score % 10, startX + DIGIT_W + 4, cy, SEG_LEN, SEG_THICK);
    } else {
        drawDigit(score, cx - DIGIT_W / 2, cy, SEG_LEN, SEG_THICK);
    }
}

void Game::drawDashedLine() {
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    int cx = SCREEN_WIDTH / 2;
    for (int y = 10; y < SCREEN_HEIGHT; y += 25) {
        drawRect(cx - 2, y, 4, 14);
    }
}

void Game::drawGameOver() {
    // Dim the whole screen
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_Rect all = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &all);

    // Highlight winner's half in green
    SDL_SetRenderDrawColor(renderer, 0, 200, 50, 60);
    SDL_Rect half = {
        winner == 1 ? 0 : SCREEN_WIDTH / 2,
        0,
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT
    };
    SDL_RenderFillRect(renderer, &half);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw winner's player number as a large golden digit
    SDL_SetRenderDrawColor(renderer, 255, 220, 0, 255);
    int bigX = (winner == 1)
        ? SCREEN_WIDTH / 4  - 30
        : 3 * SCREEN_WIDTH / 4 - 30;
    drawDigit(winner, bigX, SCREEN_HEIGHT / 2 - 80, 60, 10);

    // Small "press R" hint: draw three horizontal bars beneath the digit
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    int hintY = SCREEN_HEIGHT / 2 + 60;
    int hintX = SCREEN_WIDTH  / 2 - 60;
    for (int i = 0; i < 3; i++) {
        drawRect(hintX, hintY + i * 14, 120, 4);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Procedural audio — generates a sine-wave beep as a Mix_Chunk
// ─────────────────────────────────────────────────────────────────────────────

Mix_Chunk* Game::generateBeep(int frequencyHz, int durationMs, float volume) {
    const int sampleRate = 44100;
    const int channels   = 2; // stereo, matching Mix_OpenAudio
    int numSamples  = sampleRate * durationMs / 1000;
    int bufferSize  = numSamples * channels * static_cast<int>(sizeof(Sint16));

    Uint8* buffer = static_cast<Uint8*>(malloc(static_cast<size_t>(bufferSize)));
    if (!buffer) return nullptr;

    Sint16* samples   = reinterpret_cast<Sint16*>(buffer);
    float   amplitude = 16000.0f * volume;

    for (int i = 0; i < numSamples; i++) {
        float t   = static_cast<float>(i) / sampleRate;
        float env = 1.0f;

        // Short attack (5 ms)
        float attackEnd = sampleRate * 0.005f;
        if (i < attackEnd)
            env = static_cast<float>(i) / attackEnd;

        // Fade-out over last 20% of duration
        float releaseStart = numSamples * 0.80f;
        if (static_cast<float>(i) > releaseStart)
            env *= 1.0f - (static_cast<float>(i) - releaseStart)
                        / (static_cast<float>(numSamples) - releaseStart);

        Sint16 value = static_cast<Sint16>(
            amplitude * env * sinf(2.0f * PI * static_cast<float>(frequencyHz) * t));
        samples[i * channels]     = value; // left
        samples[i * channels + 1] = value; // right
    }

    // Memory ownership: `buffer` is allocated with malloc() so that the C
    // runtime (used by SDL_mixer internally) can free it if needed.
    // `chunk` is allocated with new and deleted in cleanup().
    // Setting allocated=0 tells SDL_mixer NOT to free abuf automatically,
    // so we retain ownership and release both in cleanup() via free()/delete.
    Mix_Chunk* chunk = new Mix_Chunk();
    chunk->allocated = 0;
    chunk->abuf      = buffer;
    chunk->alen      = static_cast<Uint32>(bufferSize);
    chunk->volume    = MIX_MAX_VOLUME;
    return chunk;
}
