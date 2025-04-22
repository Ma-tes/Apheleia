#pragma once

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../../../../src/drawing/tile.h"
#include "../../../../src/drawing/color.h"
#include "../../../../src/engine_utilities/engine_global.h"
#include "../../../../src/engine.h"
#include "../../../../src/font_system.h"
#include "../../apheleia_fluffy_global.h"
#include "../../entity.h"
#include "../../world_light.h"
#include "../../world_light_actions.h"
#include "../../tile_animator.h"
#include "../../weapon_container.h"
#include "../../map_editor/map.h"
#include "../../utilities/tile_helper.h"
#include "../../utilities/collision.h"

#define TEXT_OFFSET (vector2) { 2, 2 }

typedef void (*item_function)(global_engine_information *global);

typedef struct extended_color {
    color f_color;
    color b_color;
} extended_color;

typedef struct selection_item {
    const char *name;
  
    item_function item_action;
    vector2 position;
} selection_item;

typedef struct selection_element {
    selection_item *items;

    int items_count;
    int selected_item;

    vector2 items_position;
    int items_font_size;

    extended_color normal_color;
    extended_color selected_color;
} selection_element;

void try_update_selected_item(global_engine_information *global, selection_element *element) {
    if(element->items_count < 2) {
        element->items[element->selected_item].item_action(global);
        return;
    }

    if(global->input_handler->buttons[SDL_SCANCODE_UP].is_key_pressed) {
        element->selected_item = MOD(element->selected_item - 1, element->items_count);
    }
    if(global->input_handler->buttons[SDL_SCANCODE_DOWN].is_key_pressed) {
        element->selected_item = MOD(element->selected_item + 1, element->items_count);
    }

    element->items[element->selected_item].item_action(global);
}

void draw_selection_items(global_engine_information *global, selection_element *element) {
    int elements_count = element->items_count;
    for (int i = 0; i < elements_count; i++)
    {
        selection_item *item = &element->items[i];
        item->position = (vector2) {
            .x = element->items_position.x,
            .y = element->items_position.y + (i * (element->items_font_size * 2))
        };

        if(element->selected_item == i) {
            draw_text(item->name, item->position,
                element->selected_color.b_color, global->font, element->items_font_size);
            draw_text(item->name, add(item->position, TEXT_OFFSET),
                element->selected_color.f_color, global->font, element->items_font_size);
        }
        else {
            draw_text(item->name, item->position,
                element->normal_color.b_color, global->font, element->items_font_size);
            draw_text(item->name, add(item->position, TEXT_OFFSET),
                element->normal_color.f_color, global->font, element->items_font_size);
        }
    }
}