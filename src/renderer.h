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

void create_renderer(SDL_Renderer **renderer, SDL_Window *window) {
    (*renderer) = SDL_CreateRenderer(window, RENDERING_DRIVER, RENDERING_FLAGS);
}

void execute_render_components(global_engine_information *global) {
    SDL_SetRenderDrawColor(global->renderer, SDL_CLEAR_COLOR.r,
        SDL_CLEAR_COLOR.g, SDL_CLEAR_COLOR.b, 255);
    SDL_RenderClear(global->renderer);
    for (int i = 0; i < MAX_COMPONENT_HANDLER; i++)
    {
        global->external_components[i](global);
    }
    SDL_RenderPresent(global->renderer);
}