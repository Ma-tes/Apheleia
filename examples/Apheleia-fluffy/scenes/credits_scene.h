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

#define CREDITS_COUNT 5

static char* credits[CREDITS_COUNT] = {
    "https://bdragon1727.itch.io/fire-pixel-bullet-16x16",
    "https://arcadeisland.itch.io/guns-asset-pack-v1",
    "https://yaninyunus.itch.io/neo-zero-cyberpunk-city-tileset",
    "https://snoblin.itch.io/pixel-rpg-free-npc",
    "https://vladimir783.itch.io/pixel-shotgun-sprite-animated"
};

void back_credits_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = MAIN_MENU_SCENE;
    }
}

static selection_item credits_items[1] = {
    (selection_item) {
        .name = "BACK",
        .item_action =  back_credits_selection_action
    },
};

static selection_element credits_selection = (selection_element) {
    .items = credits_items,
    .items_count = 1,

    .items_font_size = 36,
    .items_position = (vector2) {
        .x = 200,
        .y = 805
    },

    .selected_item = 0
};
 
void initialization_credits_scene(global_engine_information *global) {
    credits_selection.normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    credits_selection.selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };
}

void update_credits_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, &credits_selection);
}

void render_credits_scene(global_engine_information *global) {
    draw_text("CREDITS", (vector2) { 290, 75 }, colors[BLUE], global->font, 56);
    draw_text("CREDITS", (vector2) { 295, 80 }, colors[WHITE], global->font, 56);

    for (int i = 0; i < CREDITS_COUNT; i++)
    {
        vector2 credit_position = (vector2) {
            .x = 190,
            .y = 250 + (i * 32)
        };
        draw_text(credits[i], credit_position, colors[WHITE], global->font, 12);
    }

    draw_selection_items(global, &credits_selection);
}