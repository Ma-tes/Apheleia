#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../../../src/drawing/tile.h"
#include "../../../src/drawing/color.h"
#include "../../../src/engine_utilities/engine_global.h"
#include "../../../src/engine.h"
#include "../../../src/font_system.h"
#include "../apheleia_fluffy_global.h"
#include "../entity.h"
#include "../world_light.h"
#include "../world_light_actions.h"
#include "../tile_animator.h"
#include "../weapon_container.h"
#include "../map_editor/map.h"
#include "../utilities/tile_helper.h"
#include "../utilities/collision.h"
#include "interactive_elements/selection_items.h"

void resume_pause_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = GAME_SCENE;
    }
}

void configuration_pause_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = CONFIGURATION_SCENE;
        engine_scenes[GAME_SCENE].execute_initialization = true;
    }
}

void back_pause_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = MAIN_MENU_SCENE;
        engine_scenes[GAME_SCENE].execute_initialization = true;
    }
}

static selection_item pause_items[3] = {
    (selection_item) {
        .name = "RESUME",
        .item_action = resume_pause_selection_action
    },
    (selection_item) {
        .name = "CONFIGURATION",
        .item_action = configuration_pause_selection_action
    },
    (selection_item) {
        .name = "BACK",
        .item_action = back_pause_selection_action
    },
};

static selection_element pause_selection = (selection_element) {
    .items = pause_items,
    .items_count = 3,

    .items_font_size = 36,
    .items_position = (vector2) {
        .x = 350,
        .y = 505
    },

    .selected_item = 0
};

void initialization_pause_scene(global_engine_information *global) {
    pause_selection.normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    pause_selection.selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };
}

void update_pause_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, &pause_selection);

    time_t previous_time = current_time;
    current_time = time(NULL) - start_time;

    game_state->timer_seconds = game_state->game_time - current_time;
}

void render_pause_scene(global_engine_information *global) {
    draw_text("(No)PAUSE MENU", (vector2) { 230, 95 }, colors[WHITE], global->font, 42);
    draw_text("(No)PAUSE MENU", (vector2) { 235, 100 }, colors[BLUE], global->font, 42);

    draw_log_message("Time: ", "%lu", (void*)game_state->timer_seconds, (vector2) { 398, 228 }, colors[WHITE], global->font, 24);
    draw_log_message("Time: ", "%lu", (void*)game_state->timer_seconds, (vector2) { 400, 230 }, colors[BLUE], global->font, 24);

    draw_selection_items(global, &pause_selection);
}