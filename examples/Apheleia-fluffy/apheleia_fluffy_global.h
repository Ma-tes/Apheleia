#pragma once

#include <stdbool.h>
#include <time.h>
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/drawing/tile.h"
#include "entity.h"
#include "world_light.h"

#define DRAW_RECTANGLE(global, rectangle, rectangle_color)          \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderFillRect(global->renderer, rectangle);                \

#define DRAW_BORDER_RECTANGLE(global, rectangle, rectangle_color)   \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderDrawRect(global->renderer, rectangle);                \

#define EXECUTE_DIRECTION_ANIMATION(interactive_entity, animation_direction, atlas, frames)                 \
    if(interactive_entity.base_direction != animation_direction || interactive_entity.is_entity_idle) {     \
        atlas.textures = frames[animation_direction].animation_frames;                                      \
        atlas.textures_count = frames[animation_direction].frames_limit_count;                              \
        atlas.texture_index = 0;                                                                            \
        interactive_entity.base_direction = animation_direction;                                            \
    }                                                                                                       \

#define MOD(x, y) ((x) < (0)) ? ((y) - ((-(x)) % y)) : (x) % (y)

static time_t start_time;
static time_t current_time;

typedef struct game_state_information {
    int game_time;
    int timer_seconds;

    int round_phase;
    int round_time;
} game_state_information;

static engine_state *apheleia_state;
static game_state_information *game_state;

static int collision_range = 16;
static int collision_offset = 64;
static int collision_precision = 64;

static lighting_configuration player_lighting_configuration = (lighting_configuration) {
    .range = 32,
    .offset = 1024,
    .precision = 64
};

enum {
    MAIN_MENU_SCENE = 0,
    GAME_SCENE = 1,
    SETTINGS_MENU_SCENE = 2,
    CREDITS_SCENE = 3,
    CONFIGURATION_SCENE = 4
};

static int current_scene_identificator = MAIN_MENU_SCENE;

struct {
    int map_index;
    color player_color;
    int weapon_index;
} game_configuration;


//TODO: Apply those values to the engine
static int audio_volume_level = 75;
static int engine_framerate = 120;