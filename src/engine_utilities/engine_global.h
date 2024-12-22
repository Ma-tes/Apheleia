#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../font_system.h"

//Default engine value representing maximum
//count of event handlers.
#if !defined(MAX_EVENT_HANDLER)
    #define MAX_EVENT_HANDLER 1 
#endif

//Default engine value representing maximum
//count of component handlers.
#if !defined(MAX_COMPONENT_HANDLER)
    #define MAX_COMPONENT_HANDLER 1
#endif

//Struct declaration created for external typedef function: `component_draw_f`.
typedef struct global_engine_information global_engine_information;

//Function declaration for component drawing, similar to render engine state.
typedef void (*component_draw_f)(global_engine_information *global);

/**
 * A structure representing global values for SDL rendering:
 * `SDL_Window`,
 * `SDL_Renderer`,
 * `SDL_Event`.
 * 
 * Futhermore it contains holder of components, events, inputs, fonts and
 * additional configuration/information fields.
**/
typedef struct global_engine_information {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *event;

    struct input *input_handler;
    font_information *font;
    vector2 window_size;

    //TODO: Create indexed span allocator.
    component_draw_f external_components[MAX_COMPONENT_HANDLER];

    void(*external_events[MAX_EVENT_HANDLER])(global_engine_information*);

    //TODO: Create sound_handler;
    uint framerate_limit;
    uint framerate;

    bool is_running;
    void *state_data;
} global_engine_information;