#pragma once

#include <SDL2/SDL.h>
#include <math.h>
#include "../../src/drawing/tile.h"
#include "../../src/drawing/color.h"
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/math/vector.h"
#include "entity.h"
#include "world_light.h"
#include "utilities/tile_helper.h"
#include "utilities/collision.h"

#define GRADIENT_LENGTH 3

static tile_information gradient_tiles[GRADIENT_LENGTH] = {
    (tile_information) {
        .index_position = (vector2) { 1, 10 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile
    },
    (tile_information) {
        .index_position = (vector2) { 2, 10 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile
    },
    (tile_information) {
        .index_position = (vector2) { 3, 10 },
        .type = BLOCK_TILE,
        .draw_tile_f = draw_tile
    },
};
static vector2 gradient_tile_size = (vector2) { 64, 64 };

bool execute_lighting_gradient(SDL_Renderer *renderer, vector2 entity_position,
    vector2 tile_position, vector2 light_position, lighting_configuration configuration) {

    double entity_tile_distance = distance(entity_position, tile_position);

    if(entity_tile_distance > (configuration.offset * 2)) {
        draw_tile(renderer, gradient_tiles[2],
            tile_position, colors[WHITE], gradient_tile_size, NULL);
        return true;
    }

    double light_tile_distance = distance(light_position, tile_position);
    if(entity_tile_distance > configuration.offset && (light_tile_distance > 296 && light_tile_distance < 512)) {
        draw_tile(renderer, gradient_tiles[1],
            tile_position, colors[WHITE], gradient_tile_size, NULL);
        return true;
    }
    if(entity_tile_distance > (configuration.offset / 2) && (light_tile_distance > 256 && light_tile_distance < 512)) {
        draw_tile(renderer, gradient_tiles[0],
            tile_position, colors[WHITE], gradient_tile_size, NULL);
        return true;
    }
    return false;
}

bool player_lighting_action(SDL_Renderer *renderer, vector2 origin_position, vector2 *light_positions, light_state state, map_tile tile) {
    vector2 tile_position = tile.position;

    for (int i = 0; i < state.configuration->precision; i++)
    {
        if(contains(light_positions[i], last_active_light_positions, last_active_light_positions_count)) {
            continue;
        }
        execute_lighting_gradient(renderer,
            origin_position,
            tile_position,
            light_positions[i],
            *state.configuration);
    }

    last_active_light_positions = light_positions;
    last_active_light_positions_count = state.configuration->precision;
    return true;
}

bool bullet_lighting_action(SDL_Renderer *renderer, vector2 origin_position, vector2 *light_positions, light_state state, map_tile tile) {
    return true;
}



