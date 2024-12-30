#pragma once

#include <SDL2/SDL.h>
#include "../../src/engine.h"
#include "map_editor/map.h"
#include "entity.h"
#include "utilities/tile_helper.h"

#define WORLD_LIGHT_ACTIONS_COUNT 2

typedef struct lighting_configuration {
    int range;
    int offset;
    int precision;
} lighting_configuration;

typedef struct light_state light_state;

typedef bool (*lighting_action)(SDL_Renderer *renderer, vector2 origin_position, vector2 *light_positions, light_state state, map_tile tile);

enum lighting_type_tag {
    SINGLE,
    MULTIPLE_VOID
};

typedef struct light_state {
    enum lighting_type_tag tag;
    union {
        struct {
            //TODO: Change interactive_entity to default entity type
            void **anonymous_entities;

            entity_t(*get_entity)(void *data);

            vector2 **lights_detection_positions;
            size_t count;
        };

        struct {
            entity_t *position_entity;
            vector2 *light_detection_positions;
        };
    };

    lighting_action action;
    lighting_configuration *configuration;
} light_state;

extern light_state default_light_states[WORLD_LIGHT_ACTIONS_COUNT];

static vector2 *last_active_light_positions;
static int last_active_light_positions_count;

void set_light_detection_positions(light_state *state, map_information map) {
    if(state->tag == SINGLE) {
        vector2 *single_light_tiles = get_detection_positions(
            state->position_entity->position,
            state->position_entity->size,
            map,
            state->configuration->range,
            state->configuration->offset,
            state->configuration->precision);

        state->light_detection_positions = single_light_tiles;
        return;
    }

    state->lights_detection_positions = (vector2**)malloc(sizeof(vector2*) * state->count);
    for (int i = 0; i < state->count; i++)
    {
        entity_t current_entity = state->get_entity(state->anonymous_entities[i]);
        vector2 *single_light_tiles = get_detection_positions(
            current_entity.position,
            current_entity.size,
            map,
            state->configuration->range,
            state->configuration->offset,
            state->configuration->precision);

        state->lights_detection_positions[i] = single_light_tiles;
    }
}

void draw_light_state(SDL_Renderer *renderer, light_state *state, map_tile tile) {
    if(state->tag == SINGLE) {
        state->action(renderer,
            state->position_entity->position,
            state->light_detection_positions,
            *state, tile);
        return;
    }

    for (int i = 0; i < state->count; i++)
    {
        entity_t current_entity = state->get_entity(state->anonymous_entities[i]);
        state->action(renderer,
            current_entity.position,
            state->lights_detection_positions[i],
            *state, tile);
    }
}