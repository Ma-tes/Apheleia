#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/math/vector.h"
#include "tile_animator.h"

typedef enum direction { EAST, NORTH, WEST, SOUTH } direction;
vector2 directions[4] = {
    [NORTH] = (vector2) { 0, -1 },
    [SOUTH] = (vector2) { 0, 1 },
    [WEST] = (vector2) { -1, 0 },
    [EAST] = (vector2) { 1, 0 },
};

typedef struct entity {
    vector2 position;
    vector2 size;

    vector2 collision_box_offset;
    vector2 collision_box_size;

    color color;

    union {
        tile_information *texture;
        animation_atlas *atlas;
    };
} entity_t;

typedef struct interactive_entity {
    entity_t base_entity;

    direction base_direction;
    bool is_entity_idle;
} interactive_entity_t;

SDL_RendererFlip get_relative_texture_flip(interactive_entity_t entity) {
    if(entity.base_direction == WEST) { return SDL_FLIP_VERTICAL; }
    if(entity.base_direction == NORTH) { return SDL_FLIP_HORIZONTAL; }
    if(entity.base_direction == SOUTH) { return SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL; }

    return SDL_FLIP_NONE;
}