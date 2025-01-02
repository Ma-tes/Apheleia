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

#define GOLD_COLOR (color) { .r = 255, .g = 215, .b = 0 }
#define STAR_SIZE (vector2) { 64, 64 }

static tile_information star_tile_information = INDEXED_ANIMATION_TILE(6, 0);

void play_again_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = GAME_SCENE;
    }
}

void configuration_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = CONFIGURATION_SCENE;
    }
}

void back_score_selection_action(global_engine_information *global) {
    if(global->input_handler->buttons[SDL_SCANCODE_RETURN].is_key_pressed) {
        current_scene_identificator = MAIN_MENU_SCENE;
    }
}

static selection_item score_items[3] = {
    (selection_item) {
        .name = "PLAY AGAIN",
        .item_action = play_again_selection_action
    },
    (selection_item) {
        .name = "CONFIGURATION",
        .item_action = configuration_selection_action
    },
    (selection_item) {
        .name = "BACK",
        .item_action = back_score_selection_action
    },
};

static selection_element score_selection = (selection_element) {
    .items = score_items,
    .items_count = 3,

    .items_font_size = 36,
    .items_position = (vector2) {
        .x = 350,
        .y = 605
    },

    .selected_item = 0
};
 
void draw_stars_line(SDL_Renderer *renderer, int count, vector2 position) {
    for (int i = 0; i < count; i++)
    {
        vector2 star_position = (vector2) {
            .x = position.x + (i * 64 ),
            .y = position.y
        };
        draw_tile(renderer, star_tile_information, star_position, colors[WHITE], STAR_SIZE, NULL);
    }
}

void initialization_score_scene(global_engine_information *global) {
    score_selection.normal_color = (extended_color) {
        .b_color = colors[GRAY],
        .f_color = colors[WHITE]
    };

    score_selection.selected_color = (extended_color) {
        .b_color = colors[WHITE],
        .f_color = colors[BLUE]
    };
}

void update_score_scene(global_engine_information *global, float delta_time) {
    try_update_selected_item(global, &score_selection);
}

void render_score_scene(global_engine_information *global) {
    draw_text("SCOREBOARD", (vector2) { 245, 95 }, colors[WHITE], global->font, 56);
    draw_text("SCOREBOARD", (vector2) { 250, 100 }, GOLD_COLOR, global->font, 56);

    draw_stars_line(global->renderer, 3, (vector2) { 90, 200 });
    draw_log_message("Score: ", "%d", (void*)player.score, (vector2) { 398, 228 }, colors[WHITE], global->font, 32);
    draw_log_message("Score: ", "%d", (void*)player.score, (vector2) { 400, 230 }, GOLD_COLOR, global->font, 32);
    draw_stars_line(global->renderer, 3, (vector2) { 775, 200 });

    draw_log_message("Final phase: ", "%d", (void*)game_state->round_phase, (vector2) { 400, 300 }, colors[WHITE], global->font, 16);
    draw_log_message("Health: ", "%d", (void*)player.health, (vector2) { 400, 350 }, colors[WHITE], global->font, 16);
    draw_log_message("Bullets fired: ", "%d", (void*)player.total_bullets_fired, (vector2) { 400, 400 }, colors[WHITE], global->font, 16);

    draw_selection_items(global, &score_selection);
}