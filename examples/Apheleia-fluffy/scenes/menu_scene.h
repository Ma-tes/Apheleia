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

void play_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        global->framerate_limit = engine_framerate;
        current_scene_identificator = CONFIGURATION_SCENE;
    }
}

void settings_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = SETTINGS_MENU_SCENE;
    }
}

void credits_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = CREDITS_SCENE;
    }
}

void quit_selection_action(global_engine_information *global) {
}

static selection_item main_items[4] = {
    (selection_item) {
        .name = "PLAY",
        .item_action =  play_selection_action
    },
    (selection_item) {
        .name = "SETTINGS",
        .item_action =  settings_selection_action
    },
    (selection_item) {
        .name = "CREDITS",
        .item_action =  credits_selection_action
    },
    (selection_item) {
        .name = "QUIT",
        .item_action =  quit_selection_action
    },
};

static selection_element main_selection = (selection_element) {
    .items = main_items,
    .items_count = 4,

    .items_font_size = 36,
    .items_position = (vector2) {
        .x = 400,
        .y = 405
    },

    .selected_item = 0
};
 
void initialization_menu_scene(global_engine_information *global) {
    main_selection.normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    main_selection.selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };
}

void update_menu_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, &main_selection);
}

void render_menu_scene(global_engine_information *global) {
    draw_text("SUPER", (vector2) { 420, 150 }, colors[BLUE], global->font, 32);
    draw_text("SUPER", (vector2) { 425, 155 }, colors[WHITE], global->font, 32);

    draw_text("FLUFFY", (vector2) { 300, 200 }, colors[BLUE], global->font, 72);
    draw_text("FLUFFY", (vector2) { 305, 205 }, colors[WHITE], global->font, 72);

    draw_text("https://github.com/Ma-tes", (vector2) { 305, 920 }, colors[WHITE], global->font, 16);
    draw_selection_items(global, &main_selection);
}