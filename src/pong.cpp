#include <unordered_map>
#include <chrono>
#include "pong.h"

static std::unordered_map<SDL_Keycode, bool> keysDown {}; // Keeps track of all keys that are held down
static float tickTimer {0.0f};                            // Keeps track of tick time to accurately call tick()

namespace Pong {
    static SDL_FRect player_paddle {0.0f, 0.0f, paddle_width, paddle_height};
    static SDL_FRect enemy_paddle  {0.0f, 0.0f, paddle_width, paddle_height};

    void init()
    {
        // Setup player paddle
        player_paddle.x = screen_width / 8.0f;                              // Left x-pos
        player_paddle.y = screen_height / 2.0f - paddle_height / 2.0f;      // Center y-pos

        // Setup enemy paddle
        enemy_paddle.x = screen_width - screen_width / 8.0f - paddle_width; // Right x-pos
        enemy_paddle.y = screen_height / 2.0f - paddle_height / 2.0f;       // Center y-pos
    }

    void input(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            keysDown[event.key.key] = true;  // Store true for key ID if held down
        } else if (event.type == SDL_EVENT_KEY_UP) {
            keysDown[event.key.key] = false; // Store false for key ID if up (let go)
        }
    }

    static void tick()
    {
        if (keysDown[SDLK_W] || keysDown[SDLK_UP]) {   // Hold up
            player_paddle.y -= (paddle_speed * tick_rate);
        }
        if (keysDown[SDLK_S] || keysDown[SDLK_DOWN]) { // Hold down
            player_paddle.y += (paddle_speed * tick_rate);
        }

        // Cap player's vertical position to be in bounds
        if (player_paddle.y < 0) {
            player_paddle.y = 0;
        } else if (player_paddle.y + paddle_height > screen_height) {
            player_paddle.y = screen_height - paddle_height;
        }
    }

    void update()
    {
        using namespace std::chrono;
        static auto previous = high_resolution_clock::now();
        
        auto current = high_resolution_clock::now();
        duration<float> time_span = duration_cast<duration<float>>(current - previous);
        previous = current;

        float deltaTime = time_span.count();
        tickTimer += deltaTime;
        while (tickTimer >= tick_rate) {
            tick();
            tickTimer -= tick_rate;
        }
    }

    void render(SDL_Renderer &renderer)
    {
        SDL_RenderFillRect(&renderer, &player_paddle);
        SDL_RenderFillRect(&renderer, &enemy_paddle);
    }
}