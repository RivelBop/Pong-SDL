#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <string>
#include <SDL3/SDL_main.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "pong.h"

// Rendering
static SDL_Window   *window   {nullptr};
static SDL_Renderer *renderer {nullptr};

// Audio
static MIX_Mixer *mixer       {nullptr};
static MIX_Audio *pong_sound  {nullptr};
static MIX_Audio *score_sound {nullptr};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    /* Change render/gpu driver to opengl (significant rendering performance improvement) */
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_SetHint(SDL_HINT_GPU_DRIVER, "opengl");

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Pong", Pong::screen_width, Pong::screen_height, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Initialize SDL_mixer */
    if (!MIX_Init()) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Set audio spec and create mixer device */
    SDL_AudioSpec audioSpec;
    SDL_zero(audioSpec);
    audioSpec.format = SDL_AUDIO_F32;
    audioSpec.channels = 1;
    audioSpec.freq = 44100;
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec);

    /* Load sound effects */
    std::string path {SDL_GetBasePath()};
    path.append("assets/bounce.mp3");
    pong_sound = MIX_LoadAudio(mixer, path.c_str(), true);

    path = SDL_GetBasePath();
    path.append("assets/score.mp3");
    score_sound = MIX_LoadAudio(mixer, path.c_str(), true);

    /* Initialize the Pong Game! */
    Pong::init();
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    Pong::input(*event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Pong::update();

    /* Clear the screen */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Draw to the screen */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Pong::render(*renderer);
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    MIX_DestroyAudio(pong_sound);
    MIX_DestroyAudio(score_sound);
    MIX_DestroyMixer(mixer);
    MIX_Quit();
}

void Pong::play_pong_sound()
{
    MIX_PlayAudio(mixer, pong_sound);
}

void Pong::play_score_sound()
{
    MIX_PlayAudio(mixer, score_sound);
}