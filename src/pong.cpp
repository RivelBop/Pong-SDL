#include "pong.h"

static float tickTimer = 0.0;
static SDL_FRect player_paddle;
static SDL_FRect enemy_paddle;

namespace pong {
    void init() {
        // Setup player paddle
        player_paddle.w = PADDLE_WIDTH;
        player_paddle.h = PADDLE_HEIGHT;
        player_paddle.x = SCREEN_WIDTH / 8.0f;                         // Left x-pos
        player_paddle.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f; // Center y-pos

        // Setup enemy paddle
        enemy_paddle.w = PADDLE_WIDTH;
        enemy_paddle.h = PADDLE_HEIGHT;
        enemy_paddle.x = SCREEN_WIDTH - SCREEN_WIDTH / 8.0f - PADDLE_WIDTH; // Right x-pos
        enemy_paddle.y = SCREEN_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;       // Center y-pos
    }

    static void tick() {
    }

    void update() {
        using namespace std::chrono;
        static auto previous = high_resolution_clock::now();
        
        auto current = high_resolution_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(current - previous);
        previous = current;

        float deltaTime = time_span.count();
        tickTimer += deltaTime;
        while (tickTimer >= TICK_RATE) {
            tick();
            tickTimer -= TICK_RATE;
        }
    }

    void render(SDL_Renderer *renderer) {
        SDL_RenderFillRect(renderer, &player_paddle);
        SDL_RenderFillRect(renderer, &enemy_paddle);
    }
}