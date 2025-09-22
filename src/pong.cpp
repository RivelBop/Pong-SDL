#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <string>
#include "pong.h"

static std::unordered_map<SDL_Keycode, bool> keys_down {}; // Keeps track of all keys that are held down
static float tick_timer {0.0f};                            // Keeps track of tick time to accurately call tick()

namespace Pong {
    static SDL_FRect player_paddle {0.0f, 0.0f, paddle_width, paddle_height};
    static SDL_FRect enemy_paddle  {0.0f, 0.0f, paddle_width, paddle_height};
    static SDL_FRect ball          {0.0f, 0.0f, ball_size, ball_size};
    static SDL_FRect collResult    {};
    static float     ball_dir_x    {0.0f};
    static float     ball_dir_y    {0.0f};
    
    static unsigned int player_score        {0};
    static unsigned int enemy_score         {0};
    static std::string player_score_display {};
    static std::string enemy_score_display  {};

    void init(bool resetScore)
    {
        // Setup player paddle
        player_paddle.x = screen_width / 8.0f;                              // Left x-pos
        player_paddle.y = screen_height / 2.0f - paddle_height / 2.0f;      // Center y-pos

        // Setup enemy paddle
        enemy_paddle.x = screen_width - screen_width / 8.0f - paddle_width; // Right x-pos
        enemy_paddle.y = screen_height / 2.0f - paddle_height / 2.0f;       // Center y-pos

        // Setup ball
        ball.x = screen_width / 2.0f - ball_size / 2.0f;
        ball.y = screen_height / 2.0f - ball_size / 2.0f;

        // Randomize ball direction
        using namespace std;
        srand(time(0));
        ball_dir_x = rand() % 2 ? 1.0f : -1.0f; // -1 or 1
        ball_dir_y = rand() % 2 ? 1.0f : -1.0f; // -1 or 1

        // Reset score
        if (resetScore) {
            player_score = enemy_score = 0;
            player_score_display = enemy_score_display = '0';
        }
    }

    void input(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            keys_down[event.key.key] = true;  // Store true for key ID if held down
        } else if (event.type == SDL_EVENT_KEY_UP) {
            keys_down[event.key.key] = false; // Store false for key ID if up (let go)
        }
    }

    static void tick()
    {
        // Player paddle movement
        if (keys_down[SDLK_W] || keys_down[SDLK_UP]) {   // Hold up
            player_paddle.y -= (paddle_speed * tick_rate);
        }
        if (keys_down[SDLK_S] || keys_down[SDLK_DOWN]) { // Hold down
            player_paddle.y += (paddle_speed * tick_rate);
        }

        // Cap player's vertical position to be in bounds
        if (player_paddle.y < 0) {
            player_paddle.y = 0;
        } else if (player_paddle.y + paddle_height > screen_height) {
            player_paddle.y = screen_height - paddle_height;
        }

        // Enemy paddle movement
        if (ball.x + ball.w / 2.0f > screen_width / 2.0f) {
            if (ball.y + ball.h / 2.0f > enemy_paddle.y + paddle_height / 2.0f) {
                enemy_paddle.y += (paddle_speed * tick_rate);
            } else {
                enemy_paddle.y -= (paddle_speed * tick_rate);
            }
        }

        // Cap enemy's vertical position to be in bounds
        if (enemy_paddle.y < 0) {
            enemy_paddle.y = 0;
        } else if (enemy_paddle.y + paddle_height > screen_height) {
            enemy_paddle.y = screen_height - paddle_height;
        }

        // Ball movement
        ball.x += (ball_dir_x * ball_speed * tick_rate);
        ball.y += (ball_dir_y * ball_speed * tick_rate);

        // Cap ball's vertical position to be in bounds
        if (ball.y < 0) {
            ball.y = 0;
            ball_dir_y *= -1.0f;
        } else if (ball.y + ball.h > screen_height) {
            ball.y = screen_height - ball.h;
            ball_dir_y *= -1.0f;
        }

        // Bounce ball if colliding with player
        if (ball.x > player_paddle.x + paddle_width / 2.0f && SDL_GetRectIntersectionFloat(&ball, &player_paddle, &collResult)) {
            ball_dir_x *= -1.0f;
        }

        // Bounce ball if colliding with enemy
        if (ball.x + ball.w < enemy_paddle.x + paddle_width / 2.0f && SDL_GetRectIntersectionFloat(&ball, &enemy_paddle, &collResult)) {
            ball_dir_x *= -1.0f;
        }

        // Ball goes off-screen (horizontally)
        if (ball.x < 0) {
            init(false);
            enemy_score_display = std::to_string(++enemy_score);
        } else if (ball.x + ball.w > screen_width) {
            init(false);
            player_score_display = std::to_string(++player_score);
        }

        // Restart game if someone wins
        if (player_score == 10 || enemy_score == 10) {
            init();
        }
    }

    void update()
    {
        using namespace std::chrono;
        static auto previous {high_resolution_clock::now()};
        
        auto current {high_resolution_clock::now()};
        duration<float> time_span {duration_cast<duration<float>>(current - previous)};
        previous = current;

        float delta_time {time_span.count()};
        tick_timer += delta_time;
        while (tick_timer >= tick_rate) {
            tick();
            tick_timer -= tick_rate;
        }
    }

    void render(SDL_Renderer &renderer)
    {
        SDL_RenderFillRect(&renderer, &player_paddle);
        SDL_RenderFillRect(&renderer, &enemy_paddle);
        SDL_RenderFillRect(&renderer, &ball);

        SDL_SetRenderScale(&renderer, font_scale, font_scale);

        // Render player's score
        float x {((screen_width / font_scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * player_score_display.length()) / 2 - screen_width / 4.0f / font_scale};
        float y {((screen_height / font_scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2 - screen_height / 3.0f / font_scale};
        SDL_RenderDebugText(&renderer, x, y, player_score_display.c_str());

        // Render enemy's score
        x = ((screen_width / font_scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * enemy_score_display.length()) / 2 + screen_width / 4.0f / font_scale;
        y = ((screen_height / font_scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2 - screen_height / 3.0f / font_scale;
        SDL_RenderDebugText(&renderer, x, y, enemy_score_display.c_str());

        SDL_SetRenderScale(&renderer, 1.0f, 1.0f);
    }
}