// Same as #pragma once
#ifndef PONG_H_
#define PONG_H_

#include <SDL3/SDL.h>

namespace pong {
    constexpr int SCREEN_WIDTH = 640;
    constexpr int SCREEN_HEIGHT = 480;
    constexpr float TICK_RATE = 1.0f / 20.0f;

    constexpr float PADDLE_WIDTH = 16.0f;
    constexpr float PADDLE_HEIGHT = 80.0f;

    extern SDL_FRect *player_paddle;
    extern SDL_FRect *enemy_paddle;

    void init();    // Initializes the game's elements
    void update();  // Updates the game's elements
    void dispose(); // Disposes the game's elements
}

#endif // PONG_H_