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

#define MODIFY_ADD_VECTOR(current_vector, x_offset, y_offset) (vector2) { (current_vector).x + (x_offset), (current_vector).y + (y_offset) }

#define CONFIGURATION_COUNT 4

#define MAP_SELECTION_ITEMS 2
#define COLOR_SELECTION_ITEMS 5
#define WEAPON_SELECTION_ITEMS 3
#define MENU_SELECTION_ITEMS 2

#define MAP_SELECTION_POSITION (vector2) { 100, 300 }
#define COLOR_SELECTION_POSITION (vector2) { 600, 300 }
#define WEAPON_SELECTION_POSITION (vector2) { 100, 600 }
#define MENU_SELECTION_POSITION (vector2) { 600, 600 }

static int current_configuration_index = 0;
static tile_information star_tile = INDEXED_ANIMATION_TILE(6, 0);

static selection_element map_selection = (selection_element) {
    .items_count = MAP_SELECTION_ITEMS, 

    .items_font_size = 16,
    .items_position = MAP_SELECTION_POSITION,

    .selected_item = 0
};

static selection_element color_selection = (selection_element) {
    .items_count = COLOR_SELECTION_ITEMS, 

    .items_font_size = 16,
    .items_position = COLOR_SELECTION_POSITION,

    .selected_item = 0
};

static selection_element weapon_selection = (selection_element) {
    .items_count = WEAPON_SELECTION_ITEMS, 

    .items_font_size = 16,
    .items_position = WEAPON_SELECTION_POSITION,

    .selected_item = 0
};

static selection_element menu_selection = (selection_element) {
    .items_count = MENU_SELECTION_ITEMS,

    .items_font_size = 42,
    .items_position = MENU_SELECTION_POSITION,

    .selected_item = 0
};

void map_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        game_configuration.map_index = map_selection.selected_item;
        current_configuration_index++;
    }
}

void color_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        game_configuration.player_color = colors[color_selection.selected_item];
        current_configuration_index++;
    }
}

void weapon_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        game_configuration.weapon_index = weapon_selection.selected_item;
        current_configuration_index++;
    }
}

void menu_play_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = GAME_SCENE;
        current_configuration_index = 0;
    }
}

void menu_back_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = MAIN_MENU_SCENE;
        current_configuration_index = 0;
    }
}

static selection_item maps_items[MAP_SELECTION_ITEMS] = {
    (selection_item) {
        .name = "Fair module",
        .item_action = map_selection_action
    },
    (selection_item) {
        .name = "Complex set",
        .item_action = map_selection_action
    }
};

static selection_item color_items[COLOR_SELECTION_ITEMS] = {
    (selection_item) {
        .name = "WHITE",
        .item_action = color_selection_action
    },
    (selection_item) {
        .name = "BLACK",
        .item_action = color_selection_action
    },
    (selection_item) {
        .name = "RED",
        .item_action = color_selection_action
    },
    (selection_item) {
        .name = "GREEN",
        .item_action = color_selection_action
    },
    (selection_item) {
        .name = "BLUE",
        .item_action = color_selection_action
    }
};

static selection_item weapon_items[WEAPON_SELECTION_ITEMS] = {
    (selection_item) {
        .name = "Pistol",
        .item_action = weapon_selection_action
    },
    (selection_item) {
        .name = "Rifle",
        .item_action = weapon_selection_action
    },
    (selection_item) {
        .name = "Shotgun",
        .item_action = weapon_selection_action
    }
};

static selection_item menu_items[MENU_SELECTION_ITEMS] = {
    (selection_item) {
        .name = "Play",
        .item_action = menu_play_selection_action
    },
    (selection_item) {
        .name = "Back",
        .item_action = menu_back_selection_action
    }
};

enum {
    MAP_SELECTION,
    COLOR_SELECTION,
    WEAPON_SELECTION,
    MENU_SELECTION
};
static selection_element *configuration_elements[CONFIGURATION_COUNT] = {
    [MAP_SELECTION] = &map_selection,
    [COLOR_SELECTION] = &color_selection,
    [WEAPON_SELECTION] = &weapon_selection,
    [MENU_SELECTION] = &menu_selection
};
 
void initialization_configuration_scene(global_engine_information *global) {
    configuration_elements[MAP_SELECTION]->items = maps_items;
    configuration_elements[COLOR_SELECTION]->items = color_items;
    configuration_elements[WEAPON_SELECTION]->items = weapon_items;
    configuration_elements[MENU_SELECTION]->items = menu_items;

    extended_color normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    extended_color selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };

    for (int i = 0; i < CONFIGURATION_COUNT; i++)
    {
        configuration_elements[i]->normal_color = normal_color;
        configuration_elements[i]->selected_color = selected_color;
    }
}

void update_configuration_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, configuration_elements[current_configuration_index]);
}

void render_configuration_scene(global_engine_information *global) {
    draw_text("CONFIGURATION", (vector2) { 120, 45 }, colors[BLUE], global->font, 56);
    draw_text("CONFIGURATION", (vector2) { 125, 50 }, colors[WHITE], global->font, 56);

    draw_text("MAP", MODIFY_ADD_VECTOR(MAP_SELECTION_POSITION, 45, -80), colors[GREEN], global->font, 24);
    draw_text("MAP", MODIFY_ADD_VECTOR(MAP_SELECTION_POSITION, 47, -78), colors[WHITE], global->font, 24);

    draw_text("COLOR", MODIFY_ADD_VECTOR(COLOR_SELECTION_POSITION, 45, -80), colors[GREEN], global->font, 24);
    draw_text("COLOR", MODIFY_ADD_VECTOR(COLOR_SELECTION_POSITION, 47, -78), colors[WHITE], global->font, 24);

    draw_text("WEAPON", MODIFY_ADD_VECTOR(WEAPON_SELECTION_POSITION, -15, -80), colors[GREEN], global->font, 24);
    draw_text("WEAPON", MODIFY_ADD_VECTOR(WEAPON_SELECTION_POSITION, -17, -78), colors[WHITE], global->font, 24);

    int star_position_x = (((current_configuration_index % 2)) * 550) + 250;
    int star_position_y = current_configuration_index > 1 ? 500 : 200;
    draw_tile(global->renderer, star_tile, (vector2) { star_position_x, star_position_y }, colors[WHITE], (vector2) { 64, 64 }, NULL);
 
    for (int i = 0; i < CONFIGURATION_COUNT; i++)
    {
        draw_selection_items(global, configuration_elements[i]);
    }

    for (int i = 0; i < COLOR_SELECTION_ITEMS; i++)
    {
        selection_item current_color_item = color_items[i];
        SDL_Rect color_rectangle = (SDL_Rect) {
            .x = current_color_item.position.x + 275,
            .y = current_color_item.position.y + 48,
            .w = 24,
            .h = 24
        };
        DRAW_RECTANGLE(global, &color_rectangle, colors[i]);
    }
}