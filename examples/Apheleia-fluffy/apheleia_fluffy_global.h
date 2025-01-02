#pragma once

#include <stdbool.h>
#include <time.h>
#include "../../src/engine_utilities/engine_global.h"
#include "../../src/engine.h"
#include "../../src/drawing/tile.h"
#include "entity.h"
#include "weapon_container.h"
#include "world_light.h"
#include "scene.h"

#define DRAW_RECTANGLE(global, rectangle, rectangle_color)          \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderFillRect(global->renderer, rectangle);                \

#define DRAW_BORDER_RECTANGLE(global, rectangle, rectangle_color)   \
    SDL_SetRenderDrawColor(global->renderer, rectangle_color.r,     \
        rectangle_color.g, rectangle_color.b, 255);                 \
    SDL_RenderDrawRect(global->renderer, rectangle);                \

#define EXECUTE_DIRECTION_ANIMATION(interactive_entity, animation_direction, atlas, frames)                     \
    if(interactive_entity.base_direction != animation_direction || interactive_entity.is_entity_idle) {         \
        (atlas).textures = frames[animation_direction].animation_frames;                                        \
        (atlas).textures_count = frames[animation_direction].frames_limit_count;                                \
        (atlas).texture_index = 0;                                                                              \
        interactive_entity.base_direction = animation_direction;                                                \
    }                                                                                                           \

#define MOD(x, y) ((x) < (0)) ? ((y) - ((-(x)) % y)) : (x) % (y)

#define SCENE_COUNT 7 
#define MAPS_COUNT 2

typedef struct direction_frames {
    tile_information animation_frames[4];
    int frames_limit_count;
} direction_frames;

static direction_frames idle_frames[4] = {
    [NORTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 2),
            INDEXED_ANIMATION_TILE(11, 2),
        },
        .frames_limit_count = 2
    },
    [SOUTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 0),
            INDEXED_ANIMATION_TILE(11, 0),
        },
        .frames_limit_count = 2
    },
    [WEST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 7),
            INDEXED_ANIMATION_TILE(11, 7),
        },
        .frames_limit_count = 2
    },
    [EAST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 1),
            INDEXED_ANIMATION_TILE(11, 1),
        },
        .frames_limit_count = 2
    },
};

static direction_frames running_frames[4] = {
    [NORTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 5),
            INDEXED_ANIMATION_TILE(11, 5),
            INDEXED_ANIMATION_TILE(12, 5),
            INDEXED_ANIMATION_TILE(13, 5),
        },
        .frames_limit_count = 4
    },
    [SOUTH] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 3),
            INDEXED_ANIMATION_TILE(11, 3),
            INDEXED_ANIMATION_TILE(12, 3),
            INDEXED_ANIMATION_TILE(13, 3),
        },
        .frames_limit_count = 4
    },
    [WEST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 6),
            INDEXED_ANIMATION_TILE(11, 6),
            INDEXED_ANIMATION_TILE(12, 6),
            INDEXED_ANIMATION_TILE(13, 6),
        },
        .frames_limit_count = 4
    },
    [EAST] = (direction_frames) {
        .animation_frames = {
            INDEXED_ANIMATION_TILE(10, 4),
            INDEXED_ANIMATION_TILE(11, 4),
            INDEXED_ANIMATION_TILE(12, 4),
            INDEXED_ANIMATION_TILE(13, 4),
        },
        .frames_limit_count = 4
    },
};

tile_information weapon_spawn_random_frames[9] = {
    INDEXED_ANIMATION_TILE(17, 14),
    INDEXED_ANIMATION_TILE(18, 14),
    INDEXED_ANIMATION_TILE(19, 14),
    INDEXED_ANIMATION_TILE(20, 14),
    INDEXED_ANIMATION_TILE(21, 14),
    INDEXED_ANIMATION_TILE(22, 14),
    INDEXED_ANIMATION_TILE(23, 14),
    INDEXED_ANIMATION_TILE(24, 14),
    INDEXED_ANIMATION_TILE(25, 14),
};

animation_atlas player_animation_configuration = (animation_atlas) {
    .type = REPEAT,

    .color = (color) { 255, 255, 255 },
    .key_frame_speed = 200
};

animation_atlas enemy_animation_configuration = (animation_atlas) {
    .type = REPEAT,

    .color = (color) { 255, 255, 255 },
    .key_frame_speed = 200
};

animation_atlas weapon_spawn_animation_atlas = (animation_atlas) {
    .type = REPEAT,
    .textures = weapon_spawn_random_frames,
    .textures_count = 9,

    .color = (color) { 255, 255, 255 },
    .key_frame_speed = 200
};

static time_t start_time;
static time_t current_time;

typedef struct game_state_information {
    int game_time;
    int timer_seconds;

    int round_phase;
    int round_time;
    lighting_configuration player_light;
} game_state_information;

struct {
    enum weapon_type current_weapon_type;
    vector2 visualization_position;
    vector2 spawn_position;

    vector2 visualization_size;

    int spawn_time;
    bool is_collected;
} random_weapon_item;

static engine_state *apheleia_state;
static game_state_information *game_state;


static int collision_range = 16;
static int collision_offset = 64;
static int collision_precision = 64;

static int weapon_item_spawn_length = 10;
static int bullet_score_points = 150;

static lighting_configuration player_lighting_configuration = (lighting_configuration) {
    .range = 32,
    .offset = 480,
    .precision = 64
};

enum {
    MAIN_MENU_SCENE = 0,
    GAME_SCENE = 1,
    SETTINGS_MENU_SCENE = 2,
    CREDITS_SCENE = 3,
    CONFIGURATION_SCENE = 4,
    SCORE_SCENE = 5,
    PAUSE_SCENE = 6
};

static int current_scene_identificator = MAIN_MENU_SCENE;

extern scene engine_scenes[SCENE_COUNT];
extern player_entity player;

struct {
    int map_index;
    color player_color;
    int weapon_index;
} game_configuration;

static char* map_paths[MAPS_COUNT] = {
    "map_editor/maps/Fair_module.bin",
    "map_editor/maps/Complex_set.bin"
};

//TODO: Apply those values to the engine
static int audio_volume_level = 75;
static int engine_framerate = 120;