#include "pong.h"

static Uint64 previous = SDL_GetTicks();
static float tickTimer = 0.0;

namespace pong {
    SDL_FRect *player_paddle = new SDL_FRect();
    SDL_FRect *enemy_paddle = new SDL_FRect();
    
    void init() {
        // Setup player paddle
        SDL_FRect &p = *player_paddle;
        p.w = PADDLE_WIDTH;
        p.h = PADDLE_HEIGHT;
        p.x = SCREEN_WIDTH / 8.0f;               // Left x-pos
        p.y = SCREEN_HEIGHT / 2.0f - p.h / 2.0f; // Center y-pos

        // Setup enemy paddle
        SDL_FRect &e = *enemy_paddle;
        e.w = PADDLE_WIDTH;
        e.h = PADDLE_HEIGHT;
        e.x = SCREEN_WIDTH - SCREEN_WIDTH / 8.0f - e.w; // Right x-pos
        e.y = SCREEN_HEIGHT / 2.0f - p.h / 2.0f;        // Center y-pos
    }

    void tick() {
        SDL_Log("Hey this is PONG!");
    }

    void update() {
        Uint64 current = SDL_GetTicks();
        float deltaTime = (current - previous) / 1000.0f;
        previous = current;

        tickTimer += deltaTime;
        while (tickTimer >= TICK_RATE) {
            tick();
            tickTimer -= TICK_RATE;
        }
    }

    void dispose() {
        delete player_paddle;
        delete enemy_paddle;
    }
}