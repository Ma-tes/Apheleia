#pragma once

#include <SDL2/SDL.h>
#include "drawing/texture_manager.h"
#include "drawing/color.h"
#include "engine_utilities/engine_global.h"

#define RENDERING_DRIVER -1

//Default engine value representing
//FLAGS for `SDL_Renderer`.
#if !defined(RENDERING_FLAGS)
    #define RENDERING_FLAGS SDL_RENDERER_ACCELERATED 
#endif

//Default engine value representing
//render clear color for `SDL_Renderer`.
#if !defined(SDL_CLEAR_COLOR)
    #define SDL_CLEAR_COLOR colors[BLACK]
#endif

typedef struct process_pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} process_pixel;

void dither_process(SDL_Texture **texture, global_engine_information *global) {
    const uint8_t dither_map[8][8] = {
        0, 32, 8, 40, 2, 34, 10, 42,
        48, 16, 56, 24, 50, 18, 58, 26,
        12, 44, 4, 36, 14, 46, 6, 38,
        60, 28, 52, 20, 62, 30, 54, 22,
        3, 35, 11, 43, 1, 33, 9, 41,
        51, 19, 59, 27, 49, 17, 57, 25,
        15, 47, 7, 39, 13, 45, 5, 37,
        63, 31, 55, 23, 61, 29, 53, 21
    };

    int window_width = (int)global->window_size.x;
    int window_height = (int)global->window_size.y;

    process_pixel *pixels = NULL; 
    int pitch;

    if(SDL_LockTexture(*texture, NULL, (void**)&pixels, &pitch)) {
        //TODO: Handle properly texture locking exception.
    }

    int pixel_y_position = 0;
    for (int i = 0; i < window_width * window_height; i++)
    {
        int pixel_x_position = i % window_width;
        if(pixel_x_position == 0 && i != 0) {
            pixel_y_position++;
        }

    }
    SDL_UnlockTexture(*texture);
}

#if defined(POST_PIXEL_PROCESS)
    #if !defined(POST_PIXEL_PROCESSES_COUNT)
        #define POST_PIXEL_PROCESSES_COUNT 16
    #endif

    typedef void (*post_pixel_process_f)(SDL_Texture **texture, global_engine_information *global);

    static post_pixel_process_f post_processes[POST_PIXEL_PROCESSES_COUNT] = {
        dither_process
    };
    static int processes_count = 0;

    static SDL_Texture *pre_process_texture;
#endif

void create_renderer(SDL_Renderer **renderer, SDL_Window *window) {

#if defined(POST_PIXEL_PROCESS)
    (*renderer) = SDL_CreateRenderer(window, RENDERING_DRIVER, RENDERING_FLAGS);

#else
    (*renderer) = SDL_CreateRenderer(window, RENDERING_DRIVER, RENDERING_FLAGS);
#endif
}

void execute_render_components(global_engine_information *global) {

#if defined(POST_PIXEL_PROCESS)
    if(pre_process_texture == NULL) {
        pre_process_texture = SDL_CreateTexture(global->renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING, (int)global->window_size.x, (int)global->window_size.y);

        SDL_SetTextureBlendMode(pre_process_texture, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderTarget(global->renderer, pre_process_texture);
#endif

    SDL_SetRenderDrawColor(global->renderer, SDL_CLEAR_COLOR.r,
        SDL_CLEAR_COLOR.g, SDL_CLEAR_COLOR.b, 255);
    SDL_RenderClear(global->renderer);

    for (int i = 0; i < MAX_COMPONENT_HANDLER; i++)
    {
        global->external_components[i](global);
    }

#if defined(POST_PIXEL_PROCESS)
    for (int i = 0; i < POST_PIXEL_PROCESSES_COUNT; i++)
    {
        post_processes[i](&pre_process_texture, global);
    }

    SDL_SetRenderTarget(global->renderer, NULL);

    SDL_RenderCopy(global->renderer,
        pre_process_texture,
        NULL, NULL);
#endif

    SDL_RenderPresent(global->renderer);
}