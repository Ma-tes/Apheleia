#pragma once

#include <SDL2/SDL.h>
#include "texture_manager.h"
#include "engine_global.h"
#include "state_definitions.h"

#define RENDERING_DRIVER -1

static int rendering_flags = SDL_RENDERER_ACCELERATED;

void create_renderer(SDL_Renderer **renderer, SDL_Window *window) {
    (*renderer) = SDL_CreateRenderer(window, RENDERING_DRIVER, rendering_flags);
}

void execute_render_components(global_engine_information *global) {
    //TODO: Add possible macro for defining rgba colors,
    //of individual textures.
    SDL_SetRenderDrawColor(global->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(global->renderer);
    for (int i = 0; i < MAX_COMPONENT_HANDLER; i++)
    {
        global->external_components[i](global);
    }
    SDL_RenderPresent(global->renderer);
}