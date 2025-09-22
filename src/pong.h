#ifndef PONG_H
#define PONG_H

#include <SDL3/SDL.h>

namespace Pong {
    constexpr int   screen_width  {640};
    constexpr int   screen_height {480};
    constexpr float tick_rate     {1.0f / 60.0f};

    constexpr float paddle_speed  {275.0f};
    constexpr float paddle_width  {16.0f};
    constexpr float paddle_height {80.0f};

    constexpr float ball_speed    {300.0f};
    constexpr float ball_size     {16.0f};

    constexpr float font_scale    {4.0f};

    void init(bool resetScore = true);   // Initializes the game's elements
    void input(const SDL_Event &event);  // Simple input handling
    void update();                       // Updates the game's elements
    void render(SDL_Renderer &renderer); // Renders the game's elements
}

#endif // PONG_H