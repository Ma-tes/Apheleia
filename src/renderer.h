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

#if defined(POST_PIXEL_PROCESS)
    #if !defined(POST_PIXEL_PROCESSES_COUNT)
        #define POST_PIXEL_PROCESSES_COUNT 0 
    #endif

    typedef void (*post_pixel_process_f)(SDL_Texture **texture, global_engine_information *global);

    static post_pixel_process_f post_processes[POST_PIXEL_PROCESSES_COUNT];
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