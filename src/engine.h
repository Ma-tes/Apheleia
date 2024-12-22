#pragma once 

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include "renderer.h"
#include "input.h"
#include "engine_utilities/engine_events.h"
#include "engine_utilities/engine_global.h"

//Default engine value for framerate limitation.
//It could be change by following field: `state->global_information->framerate_limit`.
#define DEFAULT_FRAMERATE_LIMITATION 60

#if !defined(SDL_INIT_FLAGS) 
    #define SDL_INIT_FLAGS SDL_INIT_VIDEO
#endif

static bool engines_running = false;

/**
 * Create default structure for `global_engine_information`.
 *
 * \param window The window, which would apply the following configuration.
 * \param widht Window widht.
 * \param height Window height.
**/
global_engine_information create_initialization_global(SDL_Window *window, int width, int height) {
    global_engine_information global_informations = {
        .window = window,
        .window_size = {
            .x = width,
            .y = height
        },
        .framerate_limit = DEFAULT_FRAMERATE_LIMITATION
    };
    return global_informations;
}

//Function declaration for initialization engine state.
typedef void(*initialization_function)(global_engine_information *global);

//Function declaration for update engine state.
typedef void(*update_function)(global_engine_information *global, float delta_time);

//Function declaration for render engine state.
typedef void(*render_function)(global_engine_information *global);

/**
 * A structure representing function engine states:
 *  `initialization_function`,
 *  `update_function`,
 *  `render_function`. 
 * 
 * An `engine_state` also contains informations about counters and ticks.
**/
typedef struct engine_state {
    initialization_function initialization_f;
    update_function update_f;
    render_function render_f;

    Uint32 last_framerate_ticks;
    u_long current_performance_counter;
    u_long last_performance_counter;

    global_engine_information *global_information;
} engine_state;

/**
 * Executes the initialization state, with additional configuration.
 * 
 * Starts `SDL_Init` with `SDL_INIT_FLAGS`(default: `SDL_INIT_VIDEO`)
 * and then initializates external_components, events and font system.
**/
static void execute_initialization_callback(engine_state *state) {
    SDL_Init(SDL_INIT_FLAGS);
    global_engine_information *global_informations = state->global_information;

    //Allocates `input_handler` with default(ZERO_VECTOR)
    //values for mouse positions.
    create_input(&global_informations->input_handler);

    //Creates `SDL_Renderer`, with related flags specified
    //as a `RENDERING_FLAGS`.
    create_renderer(&state->global_information->renderer, state->global_information->window);

    //Initializates the first rendering component.
    state->global_information->external_components[0] = state->render_f;

    //Initializates the first event.
    state->global_information->external_events[0] = input_event;

    //If font was initialized, then it sets `SDL_Renderer`
    //to `font_information`of current engine instance.
    if(state->global_information->font != NULL) {
        state->global_information->font->renderer = state->global_information->renderer;
    }

    state->current_performance_counter = SDL_GetPerformanceCounter();

    state->initialization_f(global_informations);
    state->global_information->is_running = true;
}

/**
 * USE ONLY FOR MULTIPLE WINDOWS, WITH SEPARETED RENDERERS.
 * 
 * Executes multiple initialization states, with additional configuration.
 * 
 * Starts `SDL_Init` with `SDL_INIT_FLAGS`(default: `SDL_INIT_VIDEO`)
 * and then initializates external_components, events and font system.
**/
void execute_initialization_callbacks(engine_state **states, int count) {
    engines_running = true;
    for (int i = 0; i < count; i++) {
        engine_state *current_state = states[i];
        execute_initialization_callback(current_state);
    }
}

/**
 * Executes update state, with additional frame sampling
 * and framerate difference calculation.
**/
static void execute_update_callback(engine_state *state) {
    state->last_performance_counter = state->current_performance_counter;
    state->current_performance_counter = SDL_GetPerformanceCounter();

    //TODO: Recondiser use of proper double precision floating value.
    float delta_time = (float)((state->current_performance_counter - state->last_performance_counter) / (double)SDL_GetPerformanceFrequency() * 100);
    state->update_f(state->global_information, delta_time);

    Uint32 framerate_ticks_difference = (SDL_GetTicks() - ((Uint32)state->last_framerate_ticks));
    const Uint32 limitation_delta = (1000 / state->global_information->framerate_limit);

    if(limitation_delta > framerate_ticks_difference) {
        SDL_Delay(limitation_delta - framerate_ticks_difference);
    }
}

/**
 * Recursively executes all events in `global_information->external_events`.
 * Count of events is determinated by `MAX_EVENT_HANDLER`.
**/
static void execute_event_handlers(global_engine_information *global_information) {
    int event_count = MAX_EVENT_HANDLER;
    for (int i = 0; i < event_count; i++)
    {
        (global_information->external_events[i])(global_information);
    }
}

/**
 * Starts engine process by calling `execute_initialization_callback` and
 * executing engine processing loop, with `SDL_PollEvent`.
 * 
 * After all events are handled, it executes:
 * `execute_update_callback(engine_state)`
 * `execute_render_components(global_engine_information*)`
**/
void start_engine(engine_state *state) {
    execute_initialization_callback(state);
    SDL_Event current_event;
    while(state->global_information->is_running) {
        state->last_framerate_ticks = SDL_GetTicks();
        state->global_information->input_handler->buttons[last_key_code].is_key_pressed = false;
        while(SDL_PollEvent(&current_event)) {
            state->global_information->event = &current_event;
            execute_event_handlers(state->global_information);
        }

        execute_update_callback(state);
        execute_render_components(state->global_information);
    }
}

/**
 * USE ONLY FOR MULTIPLE WINDOWS, WITH SEPARETED RENDERERS.
 * 
 * Starts multiple engine process by calling each `execute_initialization_callback` and
 * executing individual engine processing loops, with `SDL_PollEvent`.
 * 
 * After all events are handled, it executes individual:
 * `execute_update_callback(engine_state)`
 * `execute_render_components(global_engine_information*)`
 * 
 * The running state of all engines is setted by `engines_running`.
**/
void start_engines(engine_state **states, int count) {
    execute_initialization_callbacks(states, count);
    SDL_Event events[count];
    while(engines_running) {
        for (int i = 0; i < count; i++)
        {
            engine_state *current_state = states[i];

            if(!current_state->global_information->is_running) {
                continue;
            }

            current_state->last_framerate_ticks = SDL_GetTicks();
            current_state->global_information->input_handler->buttons[last_key_code].is_key_pressed = false;
            while(SDL_PollEvent(&events[i])) {
                current_state->global_information->event = &events[i];
                execute_event_handlers(current_state->global_information);
            }

            execute_update_callback(current_state);
            execute_render_components(current_state->global_information);
        }
    }
}
