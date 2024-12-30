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

#define MIN_VOLUME 0
#define MAX_VOLUME 100

#define MIN_FRAMERATE 5 
#define MAX_FRAMERATE 525

#define SETTINGS_ITERATION_VALUE 5

void audio_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_LEFT].is_key_pressed) {
        audio_volume_level = SDL_clamp(audio_volume_level - SETTINGS_ITERATION_VALUE, MIN_VOLUME, MAX_VOLUME);
    }
    if(global->input_handler->buttons[SDL_SCANCODE_RIGHT].is_key_pressed) {
        audio_volume_level = SDL_clamp(audio_volume_level + SETTINGS_ITERATION_VALUE, MIN_VOLUME, MAX_VOLUME);
    }
}

void framerate_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_LEFT].is_key_pressed) {
        engine_framerate = SDL_clamp(engine_framerate - SETTINGS_ITERATION_VALUE, MIN_FRAMERATE, MAX_FRAMERATE);
    }
    if(global->input_handler->buttons[SDL_SCANCODE_RIGHT].is_key_pressed) {
        engine_framerate = SDL_clamp(engine_framerate + SETTINGS_ITERATION_VALUE, MIN_FRAMERATE, MAX_FRAMERATE);
    }
}

void back_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = MAIN_MENU_SCENE;
    }
}

static selection_item settings_items[3] = {
    (selection_item) {
        .name = "AUDIO",
        .item_action =  audio_selection_action
    },
    (selection_item) {
        .name = "FRAMERATE",
        .item_action =  framerate_selection_action
    },
    (selection_item) {
        .name = "BACK",
        .item_action =  back_selection_action
    },
};

static selection_element settings_selection = (selection_element) {
    .items = settings_items,
    .items_count = 3,

    .items_font_size = 36,
    .items_position = (vector2) {
        .x = 200,
        .y = 405
    },

    .selected_item = 0
};
 
void initialization_settings_scene(global_engine_information *global) {
    settings_selection.normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    settings_selection.selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };
}

void update_settings_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, &settings_selection);
}

void render_settings_scene(global_engine_information *global) {
    draw_text("SETTINGS", (vector2) { 290, 200 }, colors[BLUE], global->font, 56);
    draw_text("SETTINGS", (vector2) { 295, 205 }, colors[WHITE], global->font, 56);

    draw_log_message("", "%d%%", (void*)audio_volume_level,
        add(settings_items[0].position, (vector2) { 600, 0 }), colors[WHITE], global->font, 36);
    draw_log_message("", "%d", (void*)engine_framerate,
        add(settings_items[1].position, (vector2) { 600, 0 }), colors[WHITE], global->font, 36);

    draw_selection_items(global, &settings_selection);
}