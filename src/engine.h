#pragma once 

#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include "renderer.h"
#include "input.h"
#include "engine_events.h"
#include "engine_global.h"

//The default engine value for framerate limitation.
//It could be change by following field: `state->global_information->framerate_limit`.
#define DEFAULT_FRAMERATE_LIMITATION 60

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

typedef void(*initialization_function)(global_engine_information *global);
typedef void(*update_function)(global_engine_information *global, float delta_time);
typedef void(*render_function)(global_engine_information *global);

typedef struct engine_state {
    initialization_function initialization_f;
    update_function update_f;
    render_function render_f;

    int last_framerate_ticks;
    u_long current_performance_counter;
    u_long last_performance_counter;

    global_engine_information *global_information;
} engine_state;

static void execute_initialization_callback(engine_state *state) { 
    SDL_Init(SDL_INIT_VIDEO);

    global_engine_information *global_informations = state->global_information;
    create_input(&global_informations->input_handler);
    create_renderer(&state->global_information->renderer, state->global_information->window);

    state->global_information->external_components[0] = state->render_f;
    state->global_information->external_events[0] = input_event;

    if(state->global_information->font != NULL) {
        state->global_information->font->renderer = state->global_information->renderer;
    }

    state->current_performance_counter = SDL_GetPerformanceCounter();

    state->initialization_f(global_informations);
    state->global_information->is_running = true;
}

void execute_initialization_callbacks(engine_state **states, int count) {
    engines_running = true;
    for (int i = 0; i < count; i++) {
        engine_state *current_state = states[i];
        execute_initialization_callback(current_state);
    }
}

static void execute_update_callback(engine_state *state) {
    state->last_performance_counter = state->current_performance_counter;
    state->current_performance_counter = SDL_GetPerformanceCounter();

    double delta_time = (double)(state->current_performance_counter - state->last_performance_counter) / (double)SDL_GetPerformanceFrequency() * 100;
    state->update_f(state->global_information, delta_time);

    int framerate_ticks_difference = (SDL_GetTicks() - state->last_framerate_ticks);
    const int limitation_delta = (1000 / state->global_information->framerate_limit);

    if(limitation_delta > framerate_ticks_difference) {
        SDL_Delay(limitation_delta - framerate_ticks_difference);
    }
}

static void execute_event_handlers(global_engine_information *global_information) {
    //TODO: Possible for limitation, with proper
    //allocator with indexed values.
    int event_count = MAX_EVENT_HANDLER;
    for (int i = 0; i < event_count; i++)
    {
        (global_information->external_events[i])(global_information);
    }
}

void start_engine(engine_state *state) {
    execute_initialization_callback(state);
    SDL_Event current_event;
    while(state->global_information->is_running) {
        state->last_framerate_ticks = SDL_GetTicks();
        while(SDL_PollEvent(&current_event)) {
            state->global_information->event = &current_event;
            execute_event_handlers(state->global_information);
        }

        execute_update_callback(state);
        execute_render_components(state->global_information);
    }
}


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
            while(SDL_PollEvent(&events[i])) {
                current_state->global_information->event = &events[i];
                execute_event_handlers(current_state->global_information);
            }

            execute_update_callback(current_state);
            execute_render_components(current_state->global_information);
        }
    }
}